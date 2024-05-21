/*                                                                            */
/* HTTP.cpp                                                                   */
/* mpuig-ma <mpuig-ma@student.42barcelona.com>                                */
/* Tue Apr  9 16:50:09 2024                                                   */

#include "HTTP.hpp"
#include <string>

static int parse_start_line ( HTTP &, std::string );
static int parse_method ( t_request &, std::string & );
static int parse_target ( t_request &, std::string & );
static int parse_http_version ( t_request &, std::string & );
static std::string parse_query ( std::string & );
static int parse_headers ( HTTP &, const std::string & );
//static int parse_body ( HTTP &, const std::string &, std::size_t );
static int parse_body ( HTTP & );
static size_t how_many_methods ( t_http_method * ptr );
static size_t get_method_longest_len ( t_http_method * ptr );

int
HTTP::parse ( void )
{
	std::string line;
	std::string::size_type pos;

	pos = this->_buffer_recv.find( LF );
	if ( pos == std::string::npos && this->_state != PENDING_BODY )
		return ( EXIT_SUCCESS );
	if ( this->_state == PENDING_START_LINE )
	{
		line = this->_buffer_recv.substr( 0, pos );
		this->_buffer_recv.erase( 0, pos );
		if ( parse_start_line( *this, line ) == EXIT_FAILURE )
			return ( EXIT_FAILURE );
	}
	else if ( this->_state == PENDING_HEADERS )
	{
		line = this->_buffer_recv.substr( 0, pos );
		this->_buffer_recv.erase( 0, pos );
		if ( line == "\r" || line.empty() )
			this->_state = PENDING_BODY;
		else if ( parse_headers( *this, line ) == EXIT_FAILURE )
			return ( EXIT_FAILURE );
	}
	else if ( this->_state == PENDING_BODY )
	{
		if ( parse_body( *this ) == EXIT_FAILURE )
			return ( EXIT_FAILURE );
	}
	return ( EXIT_SUCCESS );
}

// From RFT9112:
//
// start-line = request-line / status-line
// request-line = method SP request-target SP HTTP-version

static int
parse_start_line ( HTTP & http, std::string line )
{
	t_response & response = http.getResponse();
	t_request & request = http.getRequest();

	response.status_code = parse_method( request, line );
	if ( response.status_code != EXIT_SUCCESS )
		return ( EXIT_FAILURE );
	line.erase( 0, line.find_first_of( SP, 0 ) + 1 );
	response.status_code = parse_target( request, line );
	if ( response.status_code != EXIT_SUCCESS )
		return ( EXIT_FAILURE );
	line.erase( 0, line.find_first_of( SP, 0 ) + 1 );
	response.status_code = parse_http_version( request, line );
	if ( response.status_code != EXIT_SUCCESS )
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
	size_t                  iterator;

	pos = line.find_first_of( SP, 0 );
	if ( pos == std::string::npos )
		return ( BAD_REQUEST );
	value = line.substr( 0, pos );
	if ( value.length() > get_method_longest_len( &HTTP::methods[0] ) )
		return ( NOT_IMPLEMENTED );
	for ( iterator = 0; iterator < how_many_methods( &HTTP::methods[0] );
			++iterator )
	{
		if ( value.compare( HTTP::methods[iterator].method ) == 0 )
			break ;
	}
	if ( iterator == how_many_methods( &HTTP::methods[0] ) )
		return ( NOT_IMPLEMENTED );
	request.method = &HTTP::methods[iterator];
	return ( EXIT_SUCCESS );
}

// request-target = origin-form
//
// No other types of request-target are implemented.
// These could be, absolute-form, authority-form, asterisk-form.
//
// All `/' found at the end are removed.

int
parse_target( t_request & request, std::string & line )
{
	std::string::size_type	pos;
	std::string				value;

	pos = line.find_first_of( SP, 0 );
	if ( pos == std::string::npos )
		return ( BAD_REQUEST );
	value = line.substr( 0, pos );
	if ( value[0] != '/' )
		return ( EXIT_FAILURE );
	request.query = parse_query( value );
	if ( request.query.length() > 0 )
		value.erase( value.length() - request.query.length() );
	if ( value.back() == '/' )
	{
		while ( !value.empty() && value.back() == '/' )
		{
			value.erase( value.length() - 1, 1 );
		}
		value.append( "/" );
	}
	request.target = value;
	urldecode( request.target );
	return ( EXIT_SUCCESS );
}

// As of PROGRAM_NAME is only meant to work with HTTP/1.1
// this call check if `line' ends with CR and
// compares it's value with the supported HTTP version.

int
parse_http_version ( t_request & request, std::string & line )
{
	std::string::size_type  pos;
	std::string             value;

	pos = line.find_first_of( CR, 0 );
	if ( pos == std::string::npos )
		return ( BAD_REQUEST );
	value = line.substr( 0, pos );
	if ( value.compare( "HTTP/1.1" ) == 0)
		request.http_version = HTTP_11;
	else if ( value.compare( "HTTP/1.0" ) != 0 )
		request.http_version = HTTP_10;
	else
		return ( BAD_REQUEST );
	return ( EXIT_SUCCESS );
}

// From RFC3986 section 3.4
// https://www.rfc-editor.org/rfc/rfc3986#section-3.4
//
// The query component is indicated by the first question
// mark ("?") character and terminated by a number sign ("#") character
// or by the end of the URI.

std::string
parse_query( std::string & target )
{
	std::string value;
	std::string::size_type pos, end;

	pos = target.find( '?' );
	if ( pos != std::string::npos )
	{
		end = target.find( '#' );
		if ( end == std::string::npos )
			end = target.length();
		value = target.substr( pos, end - pos );
	}
	return ( value );
}

int
parse_headers ( HTTP & http , const std::string & line)
{
	t_request & request = http.getRequest();
	std::string field_name, field_value;
	std::string::size_type pos, len;

   	len = line.length();
   	pos = line.find_first_of( ":" );
    if ( len == 0 )
   	field_name = line.substr( 0, pos );
   	(void) strtolower( field_name );
   	++pos;
   	if ( pos != len )
   	{
  		field_value = line.substr( pos, len - pos );
  		trim_f( field_value, &std::isspace );
   	}
   	request.headers.insert( request.headers.end(),
 			std::pair< std::string, std::string> ( field_name, field_value ) );
	return ( EXIT_SUCCESS );
}

// From RFC3986 section 3.5
// https://www.rfc-editor.org/rfc/rfc3986#section-3.5
//
// A fragment identifier component is indicated by the presence of a
// number sign ("#") character and terminated by the end of the URI.
// [...]
// the fragment identifier is not used in the scheme-specific
// processing of a URI; instead, the fragment identifier is separated
// from the rest of the URI prior to a dereference, and thus the
// identifying information within the fragment itself is dereferenced
// solely by the user agent, regardless of the URI scheme.
//
// This translates to: the fragment is not send to the server.

// No whitespace is allowed between the field name and colon.
// A server MUST reject, with a response status code of 400 (Bad Request),
// any received request message that contains whitespace between
// a header field name and colon.

/*
int
HTTP::parse_field_line ( std::string & line )
{
	std::string field_name, field_value;
	std::string::size_type pos, len;

	// isgraph
	len = line.length();
	pos = line.find_first_of( ":" );
	field_name = line.substr( 0, pos );
	(void) strtolower( field_name );
	++pos;
	if ( pos != len )
	{
		field_value = line.substr( pos, len - pos );
		trim_f( field_value, &std::isspace );
	}
	this->_request.headers.insert( this->_request.headers.end(),
			std::pair< std::string, std::string> ( field_name, field_value ) );
	return ( EXIT_SUCCESS );
}
*/

static int
parse_body ( HTTP & http )
{
	(void) http;
	return ( EXIT_SUCCESS );
}

/*
static int
parse_body ( HTTP & http, const std::string & buffer, size_t pos )
{
	t_request &			request = http.getRequest();
	const t_headers &	headers = request.headers;
	std::size_t			len;

	len = 0;
	if ( pos >= buffer.length() )
		return ( EXIT_SUCCESS );
	if ( headers.find( "content-length" ) != headers.end() )
	{
		len = std::atoi( headers.at( "content-length" ).c_str() );
		request.body.assign( buffer.c_str(), pos, len );
	}
	else if ( headers.find( "transfer-encoding" ) != headers.end() )
	{
		request.body.assign( buffer.c_str(), pos, std::string::npos );
		if ( headers.at( "transfer-encoding" ) == "chunked" )
		{
			//handle_chunk( request.body );
			return ( EXIT_SUCCESS );
		}
	}
	else
	{
		http.setStatusCode( BAD_REQUEST );
		http.getResponse().headers["connection"] = "close";
	}
	return ( EXIT_SUCCESS );
}
*/

// The how_many_methods() and get_method_longest_len() calls
// iterates through the methods stored in a t_http_method and
// returns the numbers of iterations done and
// the longest method's length found, respectively.

static size_t
how_many_methods( t_http_method * ptr )
{
	size_t n;

	n = 0;
	while ( ptr != 0x0 && ptr->method != 0x0 )
	{
		++n;
		++ptr;
	}
	return ( n );
}

static size_t
get_method_longest_len ( t_http_method * ptr )
{
	size_t n;

	n = 0;
	while ( ptr->method != NULL )
	{
		if ( std::strlen( ptr->method ) > n )
			n = std::strlen( ptr->method );
		++ptr;
	}
	return ( n );
}
