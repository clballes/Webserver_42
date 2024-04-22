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
	_server( router_instance.getDefaultServer() )
{
	this->cgi_ptr = NULL;
	std::memset( &this->_request, 0x0, sizeof( this->_request ) );
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
	if ( (ssize_t) this->_buffer_recv.capacity() <= data )
		this->_buffer_recv.resize( data + 1 );
	n = recv( this->_socket_fd, (char *) this->_buffer_recv.data(), data, 0 );
	if ( n == 0 )
	{
		INFO( "Client closed connection (fd=" << this->_socket_fd << ")" );
		delete this;
		return ( EXIT_SUCCESS );
	}
	else if ( n == -1 )
	{
		WARN( "fd=" << this->_socket_fd << ": " << std::strerror( errno ) );
		return ( EXIT_FAILURE );
	}
	LOG_BUFFER( this->_buffer_recv.c_str() );
	if ( this->parse() == EXIT_FAILURE )
	{
		WARN( "Something went wrong while parsing HTTP recv" );
		return ( EXIT_FAILURE );
	}
	
	if ( this->_request_headers.find( "host" ) != this->_request_headers.end() )
		this->_request.host = this->_request_headers["host"];
	this->_server = this->_router.getServer( this->_request.host,
			this->_connection.getHost(), this->_connection.getPort() );
	
	if ( this->_request.method == 0x0 )
		this->_status_code = INTERNAL_SERVER_ERROR;
	else
	{
		strncpm()	
		size_t res = this->_request.target.find("/");
		std::string line = this->_request.target.substr(, );
		// 		Define a directory or a file from where the file should be searched (for example,
		// if url /kapouet is rooted to /tmp/www, url /kapouet/pouic/toto/pouet is
		// /tmp/www/pouic/toto/pouet).
		perform();
	}
	return ( EXIT_SUCCESS );
}

void
HTTP::perform ( void )
{
	// TODO: proper reorder
	DEBUG( this->_request.target );
	if ( can_access_file( this->_request.target ) == false )
	{
		std::cout << "A0" << std::endl;

		this->_status_code = 404;
	}
	if ( this->_server.getFlag( F_AUTOINDEX, this->_request.target ) == false
			&& is_regular_file( this->_request.target ) == false )
	{
		std::cout << "A" << std::endl;
		this->_status_code = check_index();
		std::cout << this->_status_code << std::endl;
		if (this->_status_code == 200)
			load_file( *this, this->_request.target );
		this->register_send();
	}
	else if ( this->_server.getFlag( F_AUTOINDEX, this->_request.target ) == true
			&& is_regular_file( this->_request.target ) == false ) //check_regualrflie
	{
		this->_status_code = autoindex( *this ); //em torna 200 o 404
		this->register_send();
	}
	else
	{
		this->_request.method->method_func( * this );
	}
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
	this->_buffer_send.clear();
	return ( EXIT_SUCCESS );
}

int
HTTP::compose_response ( HTTP & http )
{
	//mirar aqui els errors
	if (http._status_code > 300 && http._status_code < 500)
	{
		load_file( http, http._server.getErrorPage(http._status_code) );
	}
	DEBUG( http._status_code );
	DEBUG( http._socket_fd );
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

	DEBUG( target.c_str() );
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
		std::cout << "temp:" << tempTarget << std::endl;
		tempTarget.append("/");
		tempTarget.append(*it);
		std::cout << "temp2:" << tempTarget << std::endl;

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
