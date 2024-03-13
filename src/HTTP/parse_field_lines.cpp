/*                                                                            */
/* parse_field_files.cpp                                                      */
/* mpuig-ma <mpuig-ma@student.42barcelona.com>                                */
/* Thu Mar  7 18:44:34 2024                                                   */

#include "HTTP.hpp"
#include "parse.hpp"

static int parse_field_line ( t_headers & headers, std::string & line );

/* No whitespace is allowed between the field name and colon. 
 * In the past, differences in the handling of such whitespace 
 * have led to security vulnerabilities in request routing 
 * and response handling. 
 * A server MUST reject, with a response status code of 400 (Bad Request), 
 * any received request message that contains whitespace between 
 * a header field name and colon.
 */

int
HTTP::parse_field_lines ( void )
{
	std::string::size_type  start, pos;
	std::string             line;

	LOG( "call HTTP::parse_field_lines()" );

	start = 0;
	pos = this->_buffer_recv.find_first_of( LF, 0 );
	while ( pos != std::string::npos )
	{
		std::string line = this->_buffer_recv.substr( start, pos - start );

		if ( std::isgraph( line.at( 0 ) ) != 0 )
			parse_field_line( this->_headers, line );
		else if ( ( pos - start ) != 1 )
			return ( EXIT_FAILURE );
		
		start = pos + 1;
		pos = this->_buffer_recv.find_first_of( LF, pos + 1 );
	}

	return ( EXIT_SUCCESS );
}

int
parse_field_line ( t_headers & headers, std::string & line )
{
	std::string field_name, field_value;
	std::string::size_type pos, len;

	(void) headers;
	(void) field_name;
	(void) field_value;

	// std::clog << "header: " << line << std::endl;
	len = line.length();

	pos = line.find_first_of( ":" );

	// WIP
	//if ( pos == std::string::npos
	//		|| line.find( " ", 0, pos ) != std::string::npos )
	//	return ( EXIT_FAILURE );

	field_name = line.substr( 0, pos );
	++pos;
	
	// LOG_BUFFER( field_name.c_str() );

	if ( pos == len )
		return ( EXIT_FAILURE );

	field_value = line.substr( pos, len - pos );
	trim_f( field_value, &std::isspace );

	// LOG_BUFFER( field_value.c_str() );

	headers.insert( headers.end(),
			std::pair< std::string, std::string> ( field_name, field_value ) );

	return ( EXIT_SUCCESS );
}
