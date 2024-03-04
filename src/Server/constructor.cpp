/*                                                                            */
/* constructor.cpp                                                            */
/* mpuig-ma <mpuig-ma@student.42barcelona.com>                                */
/* Wed Feb 21 15:49:35 2024                                                   */

#include "Server.hpp"

int
IEvent::kq = 0;

// Initialize static `vector' of servers

std::vector< Server * >
Server::servers;

Server::Server ( const ServerConf & instance ): good( true )
{
	LOG( "call Server::Server( const ServerConf & )" );
	LOG( "> " << instance );

	std::memcpy( &this->_address, &instance._address,
			sizeof( instance._address ) );

	return ;
}

std::ostream &
operator << ( std::ostream & os, const Server & server )
{
	os << "server socket: " << server._socket_fd;

	return (os);
}
