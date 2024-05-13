/*                                                                            */
/* HTTP.cpp                                                                   */
/* mpuig-ma <mpuig-ma@student.42barcelona.com>                                */
/* Tue Apr  9 16:50:09 2024                                                   */

#include "HTTP.hpp"

bool routeExists(const std::string& route);

t_http_method
HTTP::methods[] = {
	{ "GET", &HTTP::http_get, HTTP_GET },
	{ "HEAD", &HTTP::http_head, HTTP_HEAD },
	{ "POST", &HTTP::http_post, HTTP_POST },
	{ "PUT", &HTTP::http_put, HTTP_PUT },
	{ "DELETE", &HTTP::http_delete, HTTP_DELETE },
	{ 0, 0, 0 }
};

HTTP::HTTP ( Router & router_instance, int fd ):
	_socket_fd( 0 ),
	_router( router_instance ),
	_connection( router_instance.getConnection( fd ) ),
	_server( router_instance.getDefaultServer() ),
	_cgi_ptr( NULL ),
	_expect( false )
{
	this->_request.method = 0x0;
	std::memset( &this->_request.file_info, 0,
			sizeof( this->_request.file_info ) );
	this->_request.http_version = 0;
	this->_socket_fd = ::accept( fd,
			(struct sockaddr *) &this->_address, &this->_address_len );
	if ( this->_socket_fd == -1 || fcntl( this->_socket_fd,
			F_SETFL, O_NONBLOCK, FD_CLOEXEC ) == -1 )
	{
		ERROR( "HTTP: error creating new client" );
		return ;
	}
	DEBUG( this->_socket_fd );
	this->register_recv();
	return ;
}

HTTP::~HTTP ( void )
{
	DEBUG( this->_socket_fd );
	close( this->_socket_fd );
	return ;
}

void
HTTP::dispatch ( struct kevent & ev )
{
	DEBUG ( "ev=" << ev.ident );
	if ( ev.filter == EVFILT_READ )
		(void) this->request_recv( ev.data );
	else if ( ev.filter == EVFILT_WRITE )
		(void) this->request_send();
	return ;
}

int
HTTP::register_recv ( void )
{
	struct kevent ev;

	DEBUG( this->_socket_fd );
	EV_SET( &ev, this->_socket_fd, EVFILT_READ,
			EV_ADD | EV_ENABLE | EV_CLEAR, 0, 0, (void * ) this );
	if ( ::kevent( IEvent::kq, &ev, 1, 0x0, 0, 0 ) == -1 )
	{
		ERROR( PROGRAM_NAME << ": kevent: " << std::strerror( errno ) );
		return ( EXIT_FAILURE );
	}
	return ( EXIT_SUCCESS );
}

int
HTTP::register_send ( void )
{
	struct kevent ev;

	DEBUG( this->_socket_fd );
	EV_SET( &ev, this->_socket_fd, EVFILT_WRITE,
			EV_ADD | EV_ENABLE | EV_CLEAR | EV_ONESHOT, 0, 0, (void *) this );
	if ( ::kevent( IEvent::kq, &ev, 1, 0x0, 0, 0 ) == -1 )
	{
		ERROR( PROGRAM_NAME << " :kevent: " << std::strerror( errno ) );
		return ( EXIT_FAILURE );
	}
	return ( EXIT_SUCCESS );
}

// TODO: keep list of clients in order to delete them if Control+C

int
HTTP::request_recv ( int64_t data )
{
	ssize_t n;

	this->_buffer_recv.resize( data );
	n = recv( this->_socket_fd, (char *) this->_buffer_recv.data(), data, 0 );
	if ( n == -1 )
	{
		INFO( std::strerror( errno ) );
		delete this;
		return ( EXIT_FAILURE );
	}
	else if ( n == 0 )
	{
		INFO( "Client closed connection (fd=" << this->_socket_fd << ")" );
		delete this;
		return ( EXIT_SUCCESS );
	}

	if ( this->_expect == true )
	{
		std::cout << "entro en el expect" << std::endl;
		this->_request.body.append( this->_buffer_recv );
		// std::cout << "body:"<<  this->_request.body << std::endl;
		this->_expect = false;
		// std::cout << "body:"<< std::endl;

		handle_chunk_expect( *this );

	}
	else if ( this->parse() == EXIT_FAILURE )
	{
		WARN( "HTTP request does not comply with HTTP/1.x specification." );
		this->_buffer_recv.clear();
		this->setStatusCode( NOT_IMPLEMENTED );
		// or this->setStatusCode( BAD_REQUEST );
		this->compose_response();
		return ( EXIT_FAILURE );
	}
	// LOG_BUFFER (this->_buffer_recv, RED);

	// limit client size max body check
	// && Setting size to 0 disables checking of client request body size.
	if ( this->_server.getClientMaxBodySize( this->_request.target ) != 0
			&& ( this->_request.body.size() > this->_server.getClientMaxBodySize( this->_request.target ) ) )
	{
		this->setStatusCode( CONTENT_TOO_LARGE );
		this->compose_response();
		return ( EXIT_FAILURE );
	}

	if ( this->_request_headers.find( "host" ) != this->_request_headers.end() )
		this->_request.host = this->_request_headers["host"];
	this->_server = this->_router.getServer( this->_request.host,
			this->_connection.getHost(), this->_connection.getPort() );

	// if its an http redirection
	// lcoation block en el compose
	// append location root change if it fits location the target

	this->_request.file = this->_request.target;
	std::string location = this->_server.getRouteString( this->_request.target );
	if ( ! location.empty() && location[0] != '*' )
	{
		std::string root_location = this->_server.getRoot( location );
		root_location.append( "/" );
		this->_request.file.replace( 0 , location.length(), root_location );
	}
	else
	{
		std::string root_location = this->_server.getRoot( location );
		root_location.append( "/" );
		this->_request.file.replace( 0, 1, root_location );
	}
	if ( this->_request.file.back() == '/' )
		this->check_index();
	stat( this->_request.file.c_str(), &this->_request.file_info );

	if ( this->_request_headers.find( "expect" ) != this->_request_headers.end()
			&& this->_request_headers.at( "expect" ).empty() == false )
	{
		this->_expect = true;
		return ( EXIT_SUCCESS );
	}
	else
		this->_expect = false;
	
	return ( this->compute_response() );
}

// There is no need to check if this->_request.method is NULL
// as this check is done in the parse() call;

int
HTTP::compute_response ( void )
{
	DEBUG( "target=" << this->_request.target );
	if ( this->_server.getFlag( this->_request.method->code,
				this->_request.target ) == false )
		this->_status_code = METHOD_NOT_ALLOWED;
	else if ( ! this->_server.getCGIpass( this->_request.target ).empty() )
	{
		this->_cgi_ptr = new CGI( *this, this->_server );
		if ( this->_cgi_ptr->execute() == EXIT_SUCCESS )
			return ( EXIT_SUCCESS );
	}
	else if ( !this->_server.getRedirection( this->_request.target ).second.empty() )
	{
		this->_status_code = this->_server.getRedirection(  this->_request.target ).first;
		this->_response_headers["Location"] = this->_server.getRedirection(  this->_request.target ).second;
	}
	else
	{
		(void) this->_request.method->method_func( * this );
	}
	(void) this->compose_response();
	return ( EXIT_SUCCESS );
}

int
HTTP::compose_response ( void )
{
	DEBUG( "status_code=" << this->_status_code );
	if ( this->_status_code >= 300 && this->_status_code <= 511 )
	{
		load_file( *this, this->_server.getErrorPage( this->_status_code ) );
		//this->_response_headers["content-type"] = "text/html"; // perque no el posem?
	}
	this->_buffer_send.append( "HTTP/1.1 " );
	this->_buffer_send.append( my_to_string( this->_status_code ) );
	this->_buffer_send.append( " \r\n" );
	this->_response_headers["content-length"] = my_to_string( this->_message_body.size() );
	// Add response headers if any + ending CRLF.
	for ( t_headers::iterator it = this->_response_headers.begin();
			it != this->_response_headers.end(); ++it )
	{
		this->_buffer_send.append( it->first );
		this->_buffer_send.append( ": " );
		this->_buffer_send.append( it->second );
		this->_buffer_send.append( "\r\n" );
	}
	this->_buffer_send.append( "\r\n" );
	// Add [message body] if any.
	this->_buffer_send.append( this->_message_body );
	this->_message_body.clear();
	
	// this->request_send();
	return ( EXIT_SUCCESS );
}

int
HTTP::request_send ( void )
{
	DEBUG( "fd=" << this->_socket_fd << " bytes=" << this->_buffer_send.length() );
	::send( this->_socket_fd,
			this->_buffer_send.c_str(),
			this->_buffer_send.length(),
			0x0 );
	if ( this->_request_headers["connection"] == "close"
			|| this->_request_headers["connection"] != "keep-alive" )
	{
		delete this;
		return ( EXIT_SUCCESS );
	}

	this->_status_code = 0;
	this->_request.host.clear();
	this->_request.target.clear();
	this->_request.file.clear();
	std::memset( &this->_request.file_info, 0,
			sizeof( this->_request.file_info ) );
	this->_request.query.clear();
	this->_request.body.clear();
	this->_buffer_send.clear();
	return ( EXIT_SUCCESS );
}

int
HTTP::load_file( HTTP & http, std::string target )
{
	std::ifstream file;
	std::ifstream::pos_type pos;

	DEBUG( "file=\"" << target << "\"" );
	file.open( target, std::ios::in | std::ios::binary | std::ios::ate );
	if ( file.good() == true && file.eof() == false )
	{
		// TODO: sanity checks
		pos = file.tellg();
		file.seekg( 0, std::ios::beg );
		http._message_body.resize( pos );
		file.read( (char *) http._message_body.data(), pos );
	}
	if ( file.good() == false )
	{
		WARN( target << ": " << std::strerror( errno ) );
		return ( FORBIDDEN );
	}
	return ( OK );
}

int 
HTTP::check_index ( void )
{
	const std::vector< std::string > & vec = this->_server.getIndex( this->_request.target );
	std::string	temp;

	DEBUG( "" );
	for ( std::vector< std::string >::const_iterator it = vec.begin();
			it != vec.end(); ++it )
	{
		temp = this->_request.file;
		if ( temp.back() != '/' )
			temp.append( "/" );
		temp.append( *it );
		if ( routeExists( temp ) )
		{
			this->_request.target.append( "/" );
			this->_request.file.append( *it );
			return ( OK );
		}
	}
	return ( FORBIDDEN );
}

Server &
HTTP::getServer ( void )
{
	return ( this->_server );
}

t_request &
HTTP::getRequest ( void )
{
	return ( this->_request );
}

t_headers &
HTTP::getHeaders ( int flag )
{
	if ( flag == 0)
		return ( this->_request_headers );
	else
		
		return ( this->_response_headers );
}

void
HTTP::setMessageBody ( const std::string & message )
{
	this->_message_body.append( message );
	return ;
}

void
HTTP::setResponseHeaders ( const std::string & name,
		const std::string & value )
{
	this->_response_headers[name] = value;
	if (name == "status")
	{
		this->_status_code = std::atoi(value.c_str());
	}
	return ;
}

void
HTTP::setStatusCode ( int value )
{
	this->_status_code = value;
	return ;
}
