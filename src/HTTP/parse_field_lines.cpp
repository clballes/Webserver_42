/*                                                                            */
/* parse_field_files.cpp                                                      */
/* mpuig-ma <mpuig-ma@student.42barcelona.com>                                */
/* Thu Mar  7 18:44:34 2024                                                   */

#include "HTTP.hpp"
#include "parse.hpp"

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
	
	//LOG( "call HTTP::parse_field_line()" );
	//LOG_BUFFER( line.c_str() );

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

	this->_headers.insert( this->_headers.end(),
			std::pair< std::string, std::string> ( field_name, field_value ) );

	return ( EXIT_SUCCESS );
}
