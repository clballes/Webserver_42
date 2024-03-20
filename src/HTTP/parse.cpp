/*                                                                            */
/* parse.cpp                                                                  */
/* mpuig-ma <mpuig-ma@student.42barcelona.com>                                */
/* Mon Mar  4 15:42:42 2024                                                   */

#include "HTTP.hpp"
#include "Log.hpp"

// using namespace HTTP

int
HTTP::parse ( void )
{
	std::string::size_type  start, pos;
	std::string             line;
	std::string             line2;

	LOG( "call HTTP::parse()" );

	start = 0;
	pos = this->_buffer_recv.find_first_of( LF, 0 );
	std::cout << pos << std::endl;
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
		{
			return ( EXIT_FAILURE );
		}
		
		start = pos + 1;
		pos = this->_buffer_recv.find_first_of( LF, pos + 1 );
	}
	// ---------- FATAL BORRARR prova rapida x guardarme el body

	std::string result;
	int b = 0;
	if (strcmp(this->_request.method->method, "GET") == 0)
	{
		b = 1;
	}
	std::size_t pos2 = this->_buffer_recv.find("\r\n\r\n");
	std::cout << pos2 << "posresult" << std::endl;
    if (pos2 != std::string::npos && b != 1) {
        result = this->_buffer_recv.substr(pos2+1); // Adding 4 to include the length of the delimiter
		this->_request.body = result;
		std::cout << "AAAA:"<< this->_request.body << std::endl;
    }
	else
		std::cout << "no hay contnet body lele" << std::endl;

	return ( EXIT_SUCCESS );
}
