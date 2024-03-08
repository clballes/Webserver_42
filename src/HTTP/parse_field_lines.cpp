/*                                                                            */
/* parse_field_files.cpp                                                      */
/* mpuig-ma <mpuig-ma@student.42barcelona.com>                                */
/* Thu Mar  7 18:44:34 2024                                                   */

#include "HTTP.hpp"

int
HTTP::parse_field_lines ( void )
{
	std::string::size_type pos;
	std::string field_name, field_value;

	LOG( "call HTTP::parse_field_lines()" );
	LOG( "remaining size: " << this->_client._buffer_recv.size() );

	pos = this->_client._buffer_recv.find_first_of( LF, 0 );
	while ( pos != std::string::npos )
	{
		pos = this->_client._buffer_recv.find_first_of( LF, pos + 1 );
	}

	std::clog << this->_client._buffer_recv;

	return ( EXIT_SUCCESS );
}
