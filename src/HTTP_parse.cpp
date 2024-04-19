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
static void parse_queries ( t_request &);
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
	// parse if querys in url
	parse_queries( this->_request );
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

void
parse_queries( t_request & request )
{
	std::size_t pos = request.target.find('?');
    if (pos != std::string::npos)
	{
        // Extract the query string starting from '?', nose si necessita començar x ?
        request.query = request.target.substr(pos + 1);
		std::cout << "REQUEST QUERY: " << request.query  << std::endl;
		// if queries exist, limit them in the target, target should be without the queries
		request.target = request.target.substr(0, pos);
	}
}

int
parse_target( t_request & request, std::string & line )
{
	std::string::size_type  pos;

	pos = line.find_first_of( SP, 0 );
	if ( pos == std::string::npos )
		return ( BAD_REQUEST );

	std::cout << "url decoded is " << request.target << std::endl;
	std::string p_string = line.substr( 0, pos );
	if ( p_string.length() < request.target.length() )
		return ( FORBIDDEN );
	else if ( line.compare( 0, request.target.length() - 1, request.target ) )
		request.target.append( line.substr( request.target.length(),
					pos - request.target.length() ) );
	else
	{
		request.target.append( line.substr( 0, pos ));
	}
	
	urldecode( request.target );
	LOG( " AFTER PARSING TARGET; request.target: \"" << request.target << "\"" );
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
