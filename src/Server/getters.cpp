/*                                                                            */
/* getters.cpp                                                                */
/* mpuig-ma <mpuig-ma@student.42barcelona.com>                                */
/* Wed Feb 21 15:51:13 2024                                                   */

#include "Server.hpp"

unsigned
Server::socket ( void ) const
{
	return (this->_socket_fd);
}
