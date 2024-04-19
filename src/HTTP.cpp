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

static int get_method_longest_len ( t_http_method * );

int
HTTP::n_methods = ( sizeof( HTTP::methods ) /
		sizeof( *HTTP::methods ) ) - 1;

std::size_t
HTTP::n_longest_method = get_method_longest_len( &HTTP::methods[0] );

static int
get_method_longest_len ( t_http_method * ptr )
{
	size_t n;

	n = 0;
	while ( ptr->method != NULL )
	{
		if ( strlen( ptr->method ) > n )
			n = strlen( ptr->method );
		++ptr;
	}
	return ( n );
}

HTTP::HTTP ( Router & router_instance, int fd ):
	_socket_fd( 0 ),
	_router( router_instance ),
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
	if ( ev.flags & EVFILT_READ )
	DEBUG( ev.ident );
	if ( ev.filter == EVFILT_READ )
		this->request_recv( ev.data );
	else if ( ev.filter == EVFILT_WRITE )
		this->request_send();
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
		ERROR( PROGRAM_NAME << ": kevent: " << ::strerror( errno ) );
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
		ERROR( PROGRAM_NAME << " :kevent: " << ::strerror( errno ) );
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
	n = recv( this->_socket_fd, (char *) this->_buffer_recv.data(), data, 0 );
	if ( n == 0 )
	{
		INFO( "Client closed connection (fd=" << this->_socket_fd << ")" );
		delete this;
		return ( EXIT_FAILURE );
	}
	LOG_BUFFER( this->_buffer_recv.c_str() );
	if ( this->parse() == EXIT_FAILURE )
	{
		WARN( "Something went wrong while parsing HTTP recv" );	
		//TODO: return failure or what ? decide...
	}
	this->_buffer_recv.clear();	
	this->_server = this->_router.getServer( this->_request.host,
			this->_address.sin_addr.s_addr, this->_address.sin_port );
	if ( this->_request.method == nullptr )
		this->_status_code = INTERNAL_SERVER_ERROR;
	//TODO: location
	else
	{
		
		// fe run open x si existeix sino return 404 o access millor
		std::cout << "AAAAAAAAA" << std::endl;
		if ( this->_server.getFlag( F_AUTOINDEX ) == false
				&& is_regular_file( this->_request.target ) == false )
		{
			std::cout << "AAAAAAAAA1" << std::endl;
			this->_status_code = check_index();
			load_file( *this, this->_request.target );
			//register_sedn
		}
		else if ( this->_server.getFlag( F_AUTOINDEX ) == true
				&& is_regular_file( this->_request.target ) == false )
		{
			std::cout << "AAAAAAAAA2" << std::endl;
			this->_status_code = autoindex( *this );
			//register_send
		}
		else
		{
			std::cout << "AAAAAAAAA3" << std::endl;
			this->_request.method->method_func( * this );
		}
	}
	return ( EXIT_SUCCESS );
}

int
HTTP::request_send ( void )
{
	DEBUG( "fd=" << this->_socket_fd
			<< " bytes=" << this->_buffer_send.length() );
	DEBUG( this->_socket_fd );
	HTTP::compose_response( *this );
	this->_request.target.clear();
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
		WARN( target << ": " << ::strerror( errno ) );
		return ( FORBIDDEN );
	}
	return ( OK );
}

int 
HTTP::check_index()
{
	std::vector<std::string> vec = this->_server.getIndex();
	for (std::vector<std::string>::const_iterator it = vec.begin(); it != vec.end(); ++it)
	{
		std::string tempTarget = this->_request.target; // Create a temporary target to check existence
		tempTarget.append(*it);
		if (routeExists(tempTarget))
		{
			this->_request.target.append(*it);
			std::cout << "route exists" << this->_request.target << std::endl;
			return ( OK );
		}
		else
		{
			tempTarget.clear(); // Clear the temporary target if route does not exist
		}
	}
	return ( FORBIDDEN );
}

// Getters
std::string
HTTP::getCGIpass( void )
{
	return ( this->_server.getCGIpass() );
}

void	HTTP::setMessageBody( std::string &message )
{
	this->_message_body = message;
}

t_request HTTP::getRequest( void )
{
	return this->_request;
}

t_headers HTTP::getHeaders()
{
	return this->_request_headers;
}
