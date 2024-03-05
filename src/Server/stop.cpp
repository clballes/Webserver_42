/*                                                                            */
/* stop.cpp                                                                   */
/* mpuig-ma <mpuig-ma@student.42barcelona.com>                                */
/* Tue Mar  5 10:23:53 2024                                                   */

#include "Server.hpp"

void
Server::stop ( void )
{
	delete ( this );

	return ;
}
