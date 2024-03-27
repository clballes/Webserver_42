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

	//adding request body for POST petitions
	int delimiter = this->_buffer_recv.find("\r\n\r\n");
	int len  = this->_buffer_recv.length();

	// OTHER PETITIONS EXCEPT POST
	if (delimiter + 5 == len )
	{
		return (EXIT_SUCCESS);
	}
	else
	{
		std::string hola = this->_buffer_recv.substr(delimiter + 4);
		this->_request.body = hola;
	}
	return ( EXIT_SUCCESS );
}
