/*                                                                            */
/* destructor.cpp                                                             */
/* mpuig-ma <mpuig-ma@student.42barcelona.com>                                */
/* Wed Feb 21 15:50:37 2024                                                   */

#include "Server.hpp"

Server::~Server ( void )
{
	if ( this->_socket_fd != 0 )
		close (this->_socket_fd);

	LOG( "call Server::~Server()" );

	return ;
}
