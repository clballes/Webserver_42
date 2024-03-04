/*                                                                            */
/* destructor.cpp                                                             */
/* mpuig-ma <mpuig-ma@student.42barcelona.com>                                */
/* Wed Feb 28 16:19:12 2024                                                   */

#include "Log.hpp"
#include "HTTP.hpp"

HTTP::~HTTP ( void )
{
	LOG( "call HTTP::~HTTP()" );

	return ;
}
