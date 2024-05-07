/*                                                                            */
/* HTTP.cpp                                                                   */
/* mpuig-ma <mpuig-ma@student.42barcelona.com>                                */
/* Tue Apr  9 16:50:09 2024                                                   */

#include "HTTP.hpp"

void		HTTP::handle_chunk(const std::string& body)
{
	std::string::size_type pos = 0;
    std::string::size_type chunk_length;

    while (pos < body.size()) {
        // Find the position of the next '\r\n'
        std::string::size_type next_crlf_pos = body.find("\r\n", pos);
        if (next_crlf_pos == std::string::npos) {
            std::cerr << "Invalid chunked data format\n";
            return;
        }

        std::string chunk_length_hex = body.substr(pos, next_crlf_pos - pos);
        chunk_length = strtol(chunk_length_hex.c_str(), NULL, 16);

        // Move past the '\r\n' to the start of the chunk data
        pos = next_crlf_pos + 2;

        // Extract the chunk data
        std::string chunk_data = body.substr(pos, chunk_length);
		//store the extracted chunks in the body requested
		this->_request.body += chunk_data;
        pos += chunk_length + 2;

		if (chunk_length == 0)
		{
			std::cout << " WE ARE AT THE END OF THE LINE "<< std::endl;
			//nose si cal afegir despres els trailer que son additional headers
		}
    }
}

int
HTTP::parse ( void )
{
	std::string::size_type  start, pos;
	std::string             line;

	start = 0;
	pos = this->_buffer_recv.find_first_of( LF, start );
	if ( pos == std::string::npos )
		pos = this->_buffer_recv.length();
	while ( pos != std::string::npos )
	{
	    std::cout << "lineeeeeee2 is: "<< line << std::endl;
		line = this->_buffer_recv.substr( start, pos - start );
		if ( start == 0 )
		{
			if ( parse_start_line( line ) == EXIT_FAILURE )
				return ( EXIT_FAILURE );
		}
		else if ( line.empty() || ( line.length() == 1 && line[0] == '\r' ) )
		{
			++pos;
			break ;
		}
		else if ( std::isgraph( line[0] ) != 0 )
		{
			if ( parse_field_line( line ) == EXIT_FAILURE )
				return ( EXIT_FAILURE );
		}
		else if ( ( pos - start ) != 1 )
			return ( EXIT_FAILURE );
		start = pos + 1;
		pos = this->_buffer_recv.find_first_of( LF, start );
	}

	// TODO: byte control from "content-length"
	// chunked request
	// We return if the method does not require to read
	// the contents of a potential message body.
	if ( this->_request.method->code != HTTP_POST
			&& this->_request.method->code != HTTP_PUT )
		return ( EXIT_SUCCESS );
	if ( pos != std::string::npos && pos < this->_buffer_recv.length() )
	{
		//li sumo 4 perque em tregui el 3f
		//this->_request.body = this->_buffer_recv.substr( pos );
		std::string body = this->_buffer_recv.substr( pos + 4 );
		std::map<std::string, std::string>::iterator iter = _request_headers.find("transfer-encoding");
		if (iter != _request_headers.end()) {
			if (iter->second == "chunked") {
				std::cout << "hay chunks" << std::endl;
				handle_chunk( body );
			}
			else
				std::cout << "request not supported!!" << std::endl;
		}
		return EXIT_SUCCESS;
	}
	return EXIT_SUCCESS;
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
	line.erase( 0, line.find_first_of( SP, 0 ) + 1 );
	this->_status_code = parse_target( this->_request, line );
	if ( this->_status_code != EXIT_SUCCESS )
		return ( EXIT_FAILURE );
	line.erase( 0, line.find_first_of( SP, 0 ) + 1 );
	this->_status_code = parse_http_version( this->_request, line );
	if ( this->_status_code != EXIT_SUCCESS )
		return ( EXIT_FAILURE );
	if ( line.back() != CR )
		return ( EXIT_FAILURE );
	return ( EXIT_SUCCESS );
}

static size_t how_many_methods( t_http_method * ptr );
static size_t get_method_longest_len ( t_http_method * ptr );

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

static std::string parse_query ( std::string & );

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
	while ( value.length() > 1 && value.back() == '/' )
		value.erase( value.length() - 1, 1 );
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
	if ( value.compare( "HTTP/1.1" ) != 0 )
		return ( BAD_REQUEST );
	request.http_version = HTTP_11;
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
HTTP::parse_field_line ( std::string & line )
{
	std::string field_name, field_value;
	std::string::size_type pos, len;

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
	this->_request_headers.insert( this->_request_headers.end(),
			std::pair< std::string, std::string> ( field_name, field_value ) );
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
