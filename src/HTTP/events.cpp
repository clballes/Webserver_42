/*                                                                            */
/* events.cpp                                                                 */
/* mpuig-ma <mpuig-ma@student.42barcelona.com>                                */
/* Thu Feb 29 15:18:53 2024                                                   */

#include "HTTP.hpp"

void
HTTP::dispatch ( struct kevent & ev )
{
	(void) ev;
	LOG( "call HTTP::dispatch()" );

	return ;
}
