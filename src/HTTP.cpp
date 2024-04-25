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
	// { "PUT", &HTTP::http_put, HTTP_PUT },
	{ "DELETE", &HTTP::http_delete, HTTP_DELETE },
	{ 0, 0, 0 }
};

HTTP::HTTP ( Router & router_instance, int fd ):
	_socket_fd( 0 ),
	_router( router_instance ),
	_connection( router_instance.getConnection( fd ) ),
	_server( router_instance.getDefaultServer() ),
	_cgi_ptr( NULL )
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

	DEBUG( "fd=" << this->_socket_fd << " bytes=" << data );
	this->_buffer_recv.resize( data + 1 );
	this->_buffer_recv.back() = '\0';
	n = recv( this->_socket_fd, (char *) this->_buffer_recv.data(), data, 0 );
	if ( n == -1 )
	{
		WARN( "fd=" << this->_socket_fd << ": " << std::strerror( errno ) );
		return ( EXIT_FAILURE );
	}
	else if ( n == 0 )
	{
		INFO( "Client closed connection (fd=" << this->_socket_fd << ")" );
		delete this;
		return ( EXIT_SUCCESS );
	}
	if ( this->parse() == EXIT_FAILURE )
	{
		LOG_BUFFER( this->_buffer_recv, RED );
		WARN( "HTTP request does not comply with HTTP/1.1 specification." );
		this->_buffer_recv.clear();
		return ( EXIT_FAILURE );
	}
	LOG_BUFFER( this->_buffer_recv, GREEN );
	if ( this->_request_headers.find( "host" ) != this->_request_headers.end() )
		this->_request.host = this->_request_headers["host"];
	this->_server = this->_router.getServer( this->_request.host,
			this->_connection.getHost(), this->_connection.getPort() );

	// TODO: filtrar allowed methods
	// if its an http redirection
	// lcoation block en el compose
	// append location root change if it fits location the target
	this->_request.file = this->_request.target;
	std::string location = this->_server.getRouteString( this->_request.target );
	if ( ! location.empty() )
	{
		std::string root_location = this->_server.getRoot( location );
		this->_request.file.replace( 0 , location.length(), root_location );
	}
	else
	{
		std::string root_location = this->_server.getRoot( location );
		root_location.append( "/" );
		this->_request.file.replace( 0, 1, root_location );
	}
	LOG( YELLOW << "file=" << this->_request.file );
	stat( this->_request.file.c_str(), &this->_request.file_info );
	/*
	if ( S_ISREG( this->_request.file_info.st_mode ) )
		LOG( YELLOW << this->_request.file << " is regular" );
	if ( S_ISDIR( this->_request.file_info.st_mode ) )
		LOG( YELLOW << this->_request.file << " is directory" );
	*/
	this->perform();
	return ( EXIT_SUCCESS );
}

void
HTTP::perform ( void )
{
	// TODO: proper reorder
	DEBUG( "file=\"" << this->_request.file << "\"" );
	this->_request.method->method_func( * this );
	return ;
}

int
HTTP::request_send ( void )
{
	DEBUG( "fd=" << this->_socket_fd
			<< " bytes=" << this->_buffer_send.length() );
	HTTP::compose_response( *this );
	::send( this->_socket_fd,
			this->_buffer_send.c_str(),
			this->_buffer_send.length(),
			0x0 );
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
HTTP::compose_response ( HTTP & http )
{
	if (http._status_code > 300 && http._status_code < 500)
	{
		load_file( http, http._server.getErrorPage(http._status_code) );
	}
	DEBUG( http._socket_fd );
	DEBUG( "status_code=" << http._status_code );
	// status-line
	http._buffer_send.append( "HTTP/1.1 " );
	// TODO replace to_string()
	http._buffer_send.append( std::to_string( http._status_code ) );
	http._buffer_send.append( " \r\n" );
	// TODO: replace to_string(); it's not c++98.
	//http._response_headers["content-type"] = "text/html";
	http._response_headers["content-length"] = std::to_string( http._message_body.size() );
	// Add response headers, if any.
	// + ending CRLF
	for ( t_headers::iterator it = http._response_headers.begin();
			it != http._response_headers.end(); ++it )
	{
		http._buffer_send.append( it->first );
		http._buffer_send.append( ": " );
		http._buffer_send.append( it->second );
		http._buffer_send.append( "\r\n" );
	}
	http._buffer_send.append( "\r\n" );
	// Add [message body], if any.
	http._buffer_send.append( http._message_body );
	http._message_body.clear();
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
	std::vector< std::string > & vec = this->_server.getIndex();
	for ( std::vector< std::string >::const_iterator it = vec.begin();
			it != vec.end(); ++it )
	{
		std::string tempTarget = this->_request.target;
		char lastChar = tempTarget.at( tempTarget.size() - 1 );
		if ( lastChar != '/' )
		{
			tempTarget.append("/");
		}
		tempTarget.append(*it);
		if (routeExists(tempTarget))
		{
			this->_request.target.append("/");
			this->_request.target.append(*it);
			std::cout << "route exists" << this->_request.target << std::endl;
			return ( OK );
		}
		else
		{
			tempTarget.clear();
		}
	}
	return ( FORBIDDEN );
}

std::string &
HTTP::getCGIpass ( void )
{
	return ( const_cast < std::string & >
			( this->_server.getCGIpass( this->_request.target ) ) );
}

void
HTTP::set_message_body ( std::string & message )
{
	this->_message_body = message;
	return ;
}

t_request &
HTTP::getRequest ( void )
{
	return ( this->_request );
}

t_headers &
HTTP::getHeaders ( void )
{
	return ( this->_request_headers );
}

void
HTTP::set_response_headers ( std::string arg, std::string value )
{
	this->_response_headers[ arg ] = value;
	std::cout << "set: " << arg << value << std::endl;
	return ;
}

void
HTTP::setStatusCode ( int value )
{
	this->_status_code = value;
	std::cout << "set sttaus code: " << this->_status_code << std::endl;
	return ;
}

Server &
HTTP::getServer ( void )
{
	return ( this->_server );
}
