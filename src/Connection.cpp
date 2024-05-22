/*                                                                            */
/* Connection.cpp                                                             */
/* mpuig-ma <mpuig-ma@student.42barcelona.com>                                */
/* Tue Apr 16 15:08:38 2024                                                   */

#include "Connection.hpp"

Connection::Connection ( const struct sockaddr_in & address,
		int domain, int type, int protocol ): _good( true ), _socket_fd( 0 )
{
	std::memcpy( &this->_address, &address, sizeof( address ) );
	this->_socket_fd = ::socket( domain, type, protocol );
	if ( this->_socket_fd == -1 || fcntl( this->_socket_fd,
				F_SETFL, O_NONBLOCK, FD_CLOEXEC ) == -1 )
	{
		ERROR( "socket: " << std::strerror( errno ) );
		this->_good = false;
		return ;
	}
	#ifdef REUSE_SOCKET
	int enable = 1;
	if ( setsockopt( this->_socket_fd, SOL_SOCKET, SO_REUSEADDR,
				&enable, sizeof( enable ) ) == -1
			|| setsockopt( this->_socket_fd, SOL_SOCKET, SO_REUSEPORT,
				&enable, sizeof( enable ) ) == -1 )
	{
		ERROR( "socket: " << std::strerror( errno ) );
		this->_good = false;
		return ;
	}
	#endif
	if ( ::bind( this->_socket_fd, (struct sockaddr *) &this->_address,
				sizeof( this->_address ) ) == -1 )
	{
		ERROR( "bind: " << std::strerror( errno )
				<< " (fd=" << this->_socket_fd << ")" );
		this->_good = false;
		return ;
	}
	if ( ::listen( this->_socket_fd, 0 ) == -1 )
	{
		ERROR( "listen: " << std::strerror( errno )
				<< " (fd=" << this->_socket_fd << ")" );
		this->_good = false;
		return ;
	}
	return ;
}

Connection::~Connection ( void )
{
	return ;
}

bool
Connection::good ( void ) const
{
	return ( this->_good );
}

int
Connection::getSocketFD ( void ) const
{
	return ( this->_socket_fd );
}

in_addr_t
Connection::getHost ( void ) const
{
	return ( ntohl( this->_address.sin_addr.s_addr ) );
}

in_port_t
Connection::getPort ( void ) const
{
	return ( ntohs( this->_address.sin_port ) );
}

const struct sockaddr_in &
Connection::getAddress ( void ) const
{
	return ( this->_address );
}
