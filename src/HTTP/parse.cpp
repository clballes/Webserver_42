/*                                                                            */
/* parse.cpp                                                                  */
/* mpuig-ma <mpuig-ma@student.42barcelona.com>                                */
/* Mon Mar  4 15:42:42 2024                                                   */

#include "HTTP.hpp"

int
HTTP::parse ( void )
{
	LOG( "call HTTP::parse()" );

	// - [ ] Read until end of header
	// WIP

	// Read start-line into struct,
	// then remove it.

	if ( HTTP::parse_start_line() == EXIT_FAILURE )
		return ( EXIT_FAILURE );
	else
		this->_client._buffer_recv.erase( 0,
				this->_client._buffer_recv.find_first_of( "\n" )  + 1 );

	if ( HTTP::parse_field_lines() == EXIT_FAILURE )
		return ( EXIT_FAILURE );

	return ( EXIT_SUCCESS );
}
