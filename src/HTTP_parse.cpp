/*                                                                            */
/* HTTP.cpp                                                                   */
/* mpuig-ma <mpuig-ma@student.42barcelona.com>                                */
/* Tue Apr  9 16:50:09 2024                                                   */

#include "HTTP.hpp"

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
	if (strcmp(this->_request.method->method, "GET") != 0)
	{
		//adding request body for POST petitions
		int delimiter = this->_buffer_recv.find( "\r\n\r\n" );
		int len  = this->_buffer_recv.length();

		// OTHER PETITIONS EXCEPT POST
		if (delimiter + 5 == len )
		{
			return (EXIT_SUCCESS);
		}
		else
		{
			this->_request.body = this->_buffer_recv.substr(delimiter + 4);
		}
		return ( EXIT_SUCCESS );
	}

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

int
parse_target( t_request & request, std::string & line )
{
	std::string::size_type	pos;
	std::string				value;

	pos = line.find_first_of( SP, 0 );
	if ( pos == std::string::npos )
		return ( BAD_REQUEST );
	value = line.substr( 0, pos );
	request.query = parse_query( value );
	if ( request.query.length() > 0 )
		value.erase( value.length() - request.query.length() );
	request.target = value;
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

/* From RFC3986 section 3.4
 * https://www.rfc-editor.org/rfc/rfc3986#section-3.4
 *
 * The query component is indicated by the first question
 * mark ("?") character and terminated by a number sign ("#") character
 * or by the end of the URI.
 * */

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

/* From RFC3986 section 3.5
 * https://www.rfc-editor.org/rfc/rfc3986#section-3.5
 *
 * A fragment identifier component is indicated by the presence of a
 * number sign ("#") character and terminated by the end of the URI.
 * [...]
 * the fragment identifier is not used in the scheme-specific
 * processing of a URI; instead, the fragment identifier is separated
 * from the rest of the URI prior to a dereference, and thus the
 * identifying information within the fragment itself is dereferenced
 * solely by the user agent, regardless of the URI scheme.
 *
 * This translates to: the fragment is not send to the server.
 *
 */

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
		if ( strlen( ptr->method ) > n )
			n = strlen( ptr->method );
		++ptr;
	}
	return ( n );
}
