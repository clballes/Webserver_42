/*                                                                            */
/* parse.cpp                                                                  */
/* mpuig-ma <mpuig-ma@student.42barcelona.com>                                */
/* Mon Mar  4 15:42:42 2024                                                   */

#include "HTTP.hpp"

int
HTTP::parse ( void )
{
	LOG( "call HTTP::parse" );

	// - [ ] Read until end of header
	// WIP

	// - [ ] Read start-line into struct

	if ( HTTP::parse_start_line() == EXIT_FAILURE )
	{
		LOG( "error HTTP::parse_start_line()" );
		return ( EXIT_FAILURE );
	}

	if ( HTTP::parse_field_lines() == EXIT_FAILURE )
	{
		LOG( "error HTTP::parse_field_lines()" );
		return ( EXIT_FAILURE );
	}

	return ( EXIT_SUCCESS );
}
