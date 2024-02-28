/*                                                                            */
/* constructor.cpp                                                            */
/* mpuig-ma <mpuig-ma@student.42barcelona.com>                                */
/* Wed Feb 28 16:18:46 2024                                                   */

#include "Log.hpp"
#include "HTTP.hpp"

#include <unistd.h>

HTTP::HTTP ( const char * headers, int64_t data )
{
	LOG( "call HTTP:HTTP( const char * )" );


	write( STDOUT_FILENO, headers, data );
	
	return ;
}
