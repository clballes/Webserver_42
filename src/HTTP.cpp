/*                                                                            */
/* HTTP.cpp                                                                   */
/* mpuig-ma <mpuig-ma@student.42barcelona.com>                                */
/* Tue Apr  9 16:50:09 2024                                                   */

#include "HTTP.hpp"

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
	_server( nullptr )
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
		this->request_recv( ev.data );
	else if ( ev.flags & EVFILT_WRITE )
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
			EV_ADD | EV_ENABLE | EV_ONESHOT, 0, 0, (void *) this );
	if ( ::kevent( IEvent::kq, &ev, 1, 0x0, 0, 0 ) == -1 )
	{
		ERROR( PROGRAM_NAME << " :kevent: " << ::strerror( errno ) );
		return ( EXIT_FAILURE );
	}
	return ( EXIT_SUCCESS );
}

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
	// Do HTTP method, compose message.
	if ( this->parse() == EXIT_FAILURE )
		WARN( "Something went wrong while parsing HTTP recv" );
	this->_buffer_recv.clear();
	this->_server = this->_router.getServer( this->_request.host );
	if ( this->_request.method == nullptr || this->_server == nullptr )
		this->_status_code = INTERNAL_SERVER_ERROR;
	//TODO: location
	// fn()
	else
	{
		// Do HTTP method GET/POST...
		// Each method sets _status_code, _headers, ...
		this->_request.method->method_func( * this );
	}
	// Generic response composition based on _status_code.
	HTTP::compose_response( *this );
	this->_request.target.clear();
	// Finally send _buffer_send.
	// Consider setting send() as an event.
	this->request_send();
	return ( EXIT_SUCCESS );
}

int
HTTP::request_send ( void )
{
	DEBUG( "fd=" << this->_socket_fd
			<< " bytes=" << this->_buffer_send.length() );
	//LOG_BUFFER( this->_buffer_send.c_str() );
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
HTTP::http_head ( HTTP & http )
{
	DEBUG( "" );
	if (http._server->getCGIpass().length() != 0 )
	{}
	else
	{}
	(void) http;
	return ( EXIT_SUCCESS );
}

int
HTTP::http_get ( HTTP & http )
{
	DEBUG( http._request.target );
	// If file is not accessible
	// and autoindex is on try accessing dir.
	if ( http._status_code >= 300 )
	{
		LOG( http._server->getErrorPage( http._status_code) );
		(void) HTTP::load_file( http, http._server->getErrorPage( http._status_code ) );
		return ( EXIT_FAILURE );
	}
	if ( is_regular_file( http._request.target ) == true )
	{
		http._status_code = HTTP::load_file( http, http._request.target );
	}
	else if ( http._server->getFlags() & F_AUTOINDEX ) // las server->_flags crec q es aixo pero nose
	{
		http._status_code = HTTP::autoindex( http );
	}
	else
		http._status_code = INTERNAL_SERVER_ERROR;

	if ( http._server->getCGIpass().length() != 0 )
	{
		http.cgi_ptr = new CGI( http );
		if ( http.cgi_ptr->execute() == EXIT_FAILURE )
			return ( EXIT_FAILURE );
		http._status_code = 200;
	}
	else
	{
		// no content status code
	}
	// implement loop fashion
	return ( EXIT_SUCCESS );
}

void HTTP::generateHTML()
{
    std::string page;
    page.append( "<!DOCTYPE html><html lang='en'><meta charset='UTF-8'><meta name='viewport' content='width=device-width, initial-scale=1.0'>" );
    page.append( "<title>Post Response</title>" );
    // Add CSS styling
    page.append( "<style>" );
    page.append( "body { font-family: Arial, sans-serif; background-color: #f4f4f4; }" );
    page.append( ".container { max-width: 800px; margin: 20px auto; padding: 20px; background-color: #fff; border-radius: 10px; box-shadow: 0 2px 5px rgba(0,0,0,0.1); }" );
    page.append( "h1 { color: #333; text-align: center; }" );
    page.append( "h2 { color: #555; text-align: center; }" );
    page.append( "p { margin: 10px 0; text-align: center; }" );
    page.append( "</style>" );
    page.append( "</head>" );
    page.append( "<body>" );
    // Add a container div for better styling
    page.append( "<div class='container'>" );
    page.append( "<h1> POST RUNNING SUCCESSFULLY </h1>" );
    page.append( "<h2> CONGRATULATIONS, YOUR DATA HAS BEEN SAVED :) </h2>" );
    // Parsing the HTTP request body to generate response
    size_t startPos = 0;
    size_t endPos = 0;
	std::string decoded_request = urldecode( this->_request.body );
	while ( ( endPos = decoded_request.find( '&', startPos ) ) != std::string::npos )
	{
		std::string substr = decoded_request.substr( startPos, endPos - startPos );
		// Process substring: replace = with :
		size_t equalPos = substr.find( '=' );
		if ( equalPos != std::string::npos )
		{
			substr.replace( equalPos, 1, ": " );
		}
		page.append( "<p>" );
		page.append( substr );
		page.append( "</p>" );
		// Move startPos to next position after '&'
		startPos = endPos + 1;
	}
    // Process the last substring (after the last '&')
    std::string lastSubstring = decoded_request.substr( startPos );
    size_t equalPos = lastSubstring.find( '=' );
    if ( equalPos != std::string::npos )
	{
        lastSubstring.replace( equalPos, 1, ": " );
    }
	page.append( "<p>" );
	page.append( lastSubstring );
	page.append( "</p>" );
    page.append( "<p>" );
    page.append( "</p>" );
    // Close container div
    page.append( "</div>" );
    page.append( "</body>" );
    page.append( "</html>" );
    this->_message_body.append( page.c_str() );
}

int
HTTP::http_post ( HTTP & http )
{
	DEBUG( "" );
	if ( http._status_code == INTERNAL_SERVER_ERROR )
	{
		LOG( http._server->getErrorPage(500) );
		(void) HTTP::load_file( http, http._server->getErrorPage(500) );
	}
	if ( http._server->getCGIpass().length() != 0 )
	{
		LOG( " Executing Cgi" );
		CGI *cgi_ptr = new CGI( http );
		if ( cgi_ptr->execute() == EXIT_FAILURE )
			return ( EXIT_FAILURE );
		http._status_code = 200;
	}
	else
	{
		http._status_code = 204;
		http._message_body.clear();
	}
	return ( EXIT_SUCCESS );
}

// TODO: pendent removal
int HTTP::put_file()
{
	std::ofstream file;

	if ( is_regular_file( this->_request.target ) )
	{
		file.open( this->_request.target.c_str() );
		file << this->_request.body;
		file.close();
		return ( 204 );
	}
	file.open( this->_request.target.c_str(), std::ofstream::out | std::ofstream::trunc );
	if ( file.is_open() == false )
		return ( 403 );
	file << this->_request.body;
	file.close();
	return ( 201 );
}

int
HTTP::http_put ( HTTP & http )
{
	DEBUG( "" );
	http._status_code = http.put_file();
	// if(http._status_code == 201) // todo ok, 201 created
	//(void) HTTP::load_file( http, http._server->_conf._error_page[404] );
	// else if(http._status_code == 204) //no content, tot ok
	//if(http._status_code == 200) podria ferla es tot ok
	// _code = writeContent(content);
	// if (_code != 201 && _code != 204)
	// 	_response = this->readHtml(_errorMap[_code]);
	// _response = head.getHeader(_response.size(), target, _code, _type, requestConf.getContentLocation(), requestConf.getLang()) + "\r\n" + _response;
	return ( EXIT_SUCCESS );
}

int
HTTP::http_delete ( HTTP & http )
{
	DEBUG( "" );
	if ( is_regular_file( http._request.target ) )
	{
		if ( remove( http._request.target.c_str() ) == 0 )
		{
			http._status_code = 200;
			http._message_body.append( "<!DOCTYPE html><body><h1>File deleted.</h1></body></html>" );
		}
		else
		{
			http._status_code = 403;
			HTTP::load_file( http, http._server->getErrorPage( 403 ) );
		}
	}
	else
	{
		http._status_code = 404;
		HTTP::load_file( http, http._server->getErrorPage( 404 ) );
	}
	return ( EXIT_SUCCESS );
}

int
HTTP::parse ( void )
{
	std::string::size_type  start, pos;
	std::string             line;

	start = 0;
	pos = this->_buffer_recv.find_first_of( LF, 0 );
	while ( pos != std::string::npos )
	{
		line = this->_buffer_recv.substr( start, pos - start );
		if ( start == 0 )
		{
			if ( parse_start_line( line ) == EXIT_FAILURE )
				return ( EXIT_FAILURE );
		}
		else if ( std::isgraph( line.at( 0 ) ) != 0 )
		{
			if ( parse_field_line( line ) == EXIT_FAILURE )
				return ( EXIT_FAILURE );
		}
		else if ( ( pos - start ) != 1 )
			return ( EXIT_FAILURE );
		start = pos + 1;
		pos = this->_buffer_recv.find_first_of( LF, pos + 1 );
	}
	// TODO: duplicate/reiterative
	// TODO: remove port (:xxx ? )
	this->_request.host.assign( this->_request_headers["host"] );
	//adding request body for POST petitions
	int delimiter = this->_buffer_recv.find( "\r\n\r\n" );
	int len  = this->_buffer_recv.length();
	// OTHER PETITIONS EXCEPT POST
	if ( delimiter + 5 == len )
		return ( EXIT_SUCCESS );
	else
		this->_request.body = this->_buffer_recv.substr( delimiter + 4 );
	return ( EXIT_SUCCESS );
}

/* No whitespace is allowed between the field name and colon.
 * A server MUST reject, with a response status code of 400 (Bad Request),
 * any received request message that contains whitespace between
 * a header field name and colon.
 */

int
HTTP::parse_field_line ( std::string & line )
{
	std::string field_name, field_value;
	std::string::size_type pos, len;

	len = line.length();
	pos = line.find_first_of( ":" );
	// TODO
	//if ( pos == std::string::npos
	//		|| line.find( " ", 0, pos ) != std::string::npos )
	//	return ( EXIT_FAILURE );
	field_name = line.substr( 0, pos );
	++pos;
	if ( pos == len )
		return ( EXIT_FAILURE );
	field_value = line.substr( pos, len - pos );
	trim_f( field_value, &std::isspace );
	strtolower( field_name );
	this->_request_headers.insert( this->_request_headers.end(),
			std::pair< std::string, std::string> ( field_name, field_value ) );
	return ( EXIT_SUCCESS );
}

// From RFT9112:
//
// start-line = request-line / status-line
// request-line = method SP request-target SP HTTP-version

static int parse_method ( t_request &, std::string & );
static int parse_target ( t_request &, std::string & );
static int parse_http_version ( t_request &, std::string & );

int
HTTP::parse_start_line( std::string & line )
{
	this->_status_code = parse_method( this->_request, line );
	if ( this->_status_code != EXIT_SUCCESS )
		return ( EXIT_FAILURE );
	else
		line.erase( 0, line.find_first_of( SP, 0 ) + 1 );
	this->_status_code = parse_target( this->_request, line );
	if ( this->_status_code != EXIT_SUCCESS )
		return ( EXIT_FAILURE );
	else
		line.erase( 0, line.find_first_of( SP, 0 ) + 1 );
	this->_status_code = parse_http_version( this->_request, line );
	if ( this->_status_code != EXIT_SUCCESS )
		return ( EXIT_FAILURE );
	if ( line.back() != CR )
		return ( EXIT_FAILURE );
	return ( EXIT_SUCCESS );
}

int
parse_method( t_request & request, std::string & line )
{
	std::string::size_type  pos;
	std::string             value;
	int                     iterator;

	pos = line.find_first_of( SP, 0 );
	if ( pos == std::string::npos )
		return ( BAD_REQUEST );
	value = line.substr( 0, pos );
	if ( value.length() > HTTP::n_longest_method )
		return ( NOT_IMPLEMENTED );
	for ( iterator = 0; iterator < HTTP::n_methods; ++iterator )
	{
		if ( value.compare( HTTP::methods[iterator].method ) == 0 )
			break ;
	}
	if ( iterator == HTTP::n_methods )
		return ( NOT_IMPLEMENTED );
	request.method = &HTTP::methods[iterator];
	return ( EXIT_SUCCESS );
}


int
parse_target( t_request & request, std::string & line )
{
	std::string::size_type  pos;

	pos = line.find_first_of( SP, 0 );
	if ( pos == std::string::npos )
		return ( BAD_REQUEST );
	std::string p_string = line.substr( 0, pos );
	if ( p_string.length() < request.target.length() )
		return ( FORBIDDEN );
	else if ( line.compare( 0, request.target.length() - 1, request.target ) )
		request.target.append( line.substr( request.target.length(),
					pos - request.target.length() ) );
	else
		request.target.append( line.substr( 0, pos ) );
	urldecode( request.target );
	return ( EXIT_SUCCESS );
}

int
parse_http_version ( t_request & request, std::string & line )
{
	std::string::size_type  pos;
	std::string             value;

	pos = line.find_first_of( CR, 0 );
	if ( pos == std::string::npos )
		return ( BAD_REQUEST );
	value = line.substr( 0, pos );
	if ( value.compare( "HTTP/1.1" ) != 0 )
		return ( BAD_REQUEST );
	request.http_version = HTTP_11;
	return ( EXIT_SUCCESS );
}

// Getters

std::string
HTTP::getTarget( void )
{
	return ( this->_request.target );
}

std::string
HTTP::getCGIpass( void )
{
	return ( this->_server->getCGIpass() );
}
