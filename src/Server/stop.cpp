/*                                                                            */
/* stop.cpp                                                                   */
/* mpuig-ma <mpuig-ma@student.42barcelona.com>                                */
/* Tue Mar  5 10:23:53 2024                                                   */

#include "Server.hpp"

void
Server::stop ( void )
{
	for ( std::vector< Client * >::iterator iterator = this->_clients.begin();
			iterator != this->_clients.end(); ++iterator )
	{
		if ( *iterator != NULL )
			delete *iterator;
	}

	delete ( this );

	return ;
}
