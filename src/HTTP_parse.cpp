/*                                                                            */
/* HTTP.cpp                                                                   */
/* mpuig-ma <mpuig-ma@student.42barcelona.com>                                */
/* Tue Apr  9 16:50:09 2024                                                   */

#include "HTTP.hpp"
#include <string>

static int parse_start_line ( HTTP &, std::string );
static int parse_field_line ( HTTP &, const std::string & );
static int parse_body ( HTTP &, const std::string & );
static int handle_chunk ( HTTP &, const std::string &, std::string & );

static size_t how_many_methods ( t_http_method * ptr );
static size_t get_method_longest_len ( t_http_method * ptr );

int
HTTP::parse ( void )
{
	std::string line;
	std::string::size_type pos;

	pos = this->_buffer_recv.find( LF );
	if ( pos == std::string::npos && this->_state != PENDING_BODY )
		return ( EXIT_FAILURE );
	if ( this->_state == PENDING_START_LINE )
	{
		line = this->_buffer_recv.substr( 0, pos );
		this->_buffer_recv.erase( 0, ++pos );
		if ( parse_start_line( *this, line ) == EXIT_FAILURE )
		{
			this->setState( BAD_REQUEST );
			return ( EXIT_FAILURE );
		}
		this->_state = PENDING_HEADERS;
	}
	else if ( this->_state == PENDING_HEADERS )
	{
		line = this->_buffer_recv.substr( 0, pos );
		this->_buffer_recv.erase( 0, ++pos );
		if ( line == "\r" || line.empty() )
			this->_state = PENDING_BODY;
		else if ( parse_field_line( *this, line ) == EXIT_FAILURE )
		{
			this->setState( BAD_REQUEST );
			return ( EXIT_FAILURE );
		}
	}
	else if ( this->_state == PENDING_BODY )
	{
		if ( parse_body( *this, this->_buffer_recv ) == EXIT_FAILURE )
		{
			return ( EXIT_FAILURE );
		}
	}
	return ( EXIT_SUCCESS );
}

static int parse_method ( t_request &, std::string & );
static int parse_target ( t_request &, std::string & );
static int parse_http_version ( t_request &, std::string & );
static std::string parse_query ( std::string & );

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

static int
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

static int
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

static int
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

static std::string
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

int
parse_field_line ( HTTP & http , const std::string & line)
{
	t_request & request = http.getRequest();
	std::string field_name, field_value;
	std::string::size_type pos, len;

	//isgraph
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
   	request.headers.insert( request.headers.end(),
 			std::pair< std::string, std::string> ( field_name, field_value ) );
	return ( EXIT_SUCCESS );
}

static int
parse_body ( HTTP & http, const std::string & buffer )
{
	t_request &			request = http.getRequest();
	const t_headers &	headers = request.headers;
	std::size_t			len;

	len = 0;
	if ( headers.find( "transfer-encoding" ) != headers.end() )
	{
		if ( headers.at( "transfer-encoding" ) == "chunked" )
		{
			while ( http.getState() != COMPLETE )
			{
				if (handle_chunk( http, buffer, http.getResponse().body ) == EXIT_FAILURE)
					break;
				else
					http.setState( COMPLETE );
			}
		}
		else
		{
			http.setStatusCode( NOT_IMPLEMENTED );
		}
	}
	else if ( headers.find( "content-length" ) != headers.end() )
	{
		len = std::atoi( headers.at( "content-length" ).c_str() );
		request.body.append( buffer );
		if (request.body.length() == len)
			http.setState( COMPLETE );
		else if ( request.body.length() > len )
		{
			http.setState( BAD_REQUEST );
			return ( EXIT_FAILURE );
		}
		else
			return ( EXIT_FAILURE );
	}
	else
		http.setState( COMPLETE );
	return ( EXIT_SUCCESS );
}

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


int
handle_chunk (HTTP &  http, const std::string & buffer, std::string & body )
{
	(void)http;
	std::string::size_type	pos;
    std::string::size_type	chunk_length;

	pos = 0;
    while ( pos < buffer.size() )
	{
        std::string::size_type crlf_pos = buffer.find( "\r\n", pos );
        std::string::size_type next_crlf_pos = buffer.find( "\r\n", crlf_pos );
        if ( crlf_pos == std::string::npos ){
            return ( EXIT_FAILURE );
        }
		if ( next_crlf_pos == std::string::npos )
		{
            return ( EXIT_FAILURE );
        }

        std::string chunk_length_hex = buffer.substr( pos, crlf_pos - pos );
        chunk_length = strtol( chunk_length_hex.c_str(), NULL, 16 );
        pos = crlf_pos + 2;

        std::string chunk_data = buffer.substr( pos, chunk_length );

		body += chunk_data;
        pos += chunk_length + 2;
		if ( chunk_length == 0 )
		{
			return (EXIT_SUCCESS);
		}
    }
	return ( EXIT_SUCCESS );
}
