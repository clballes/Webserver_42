/*                                                                            */
/* clear.cpp                                                                  */
/* mpuig-ma <mpuig-ma@student.42barcelona.com>                                */
/* Wed Feb 28 12:22:29 2024                                                   */

#include "Server.hpp"

void
Server::clear ( void )
{
	Server::iterator it = Server::servers.begin();

	for ( ; it != Server::servers.end(); ++it )
		delete *it;

	return ;
}
