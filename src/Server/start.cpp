/*                                                                            */
/* start.cpp                                                                  */
/* mpuig-ma <mpuig-ma@student.42barcelona.com>                                */
/* Wed Feb 28 12:12:44 2024                                                   */

#include "Server.hpp"

int
Server::start ( void )
{
	LOG( "call Server::start()" );

	//LOG( "call socket()" );

	// Create socket ( server's )

	this->_socket_fd = ::socket( AF_INET, SOCK_STREAM, IPPROTO_TCP );

	if ( this->_socket_fd == -1
			|| fcntl( this->_socket_fd,
				F_SETFL, O_NONBLOCK, FD_CLOEXEC ) == -1 )
	{
		std::cerr << "socket: " << ::strerror( errno );
		std::cerr << std::endl;
		return ( EXIT_FAILURE );
	}

	#ifdef REUSE_SOCKET

	int enable = 1;
	if ( setsockopt( this->_socket_fd, SOL_SOCKET, SO_REUSEADDR,
				&enable, sizeof( enable ) ) == -1
			|| setsockopt( this->_socket_fd, SOL_SOCKET, SO_REUSEPORT,
				&enable, sizeof( enable ) ) == -1 )
	{
		std::cerr << "socket: " << ::strerror( errno );
		std::cerr << std::endl;
		return ( EXIT_FAILURE );
	}

	#endif

	// ::setsockopt();
	
	// Bind server's address to newly
	// create socket.

	//LOG( "call bind() (fd=" << this->_socket_fd << ")" );

	if ( ::bind( this->_socket_fd, (struct sockaddr *) &this->_address,
				sizeof( this->_address ) ) == -1 )
	{
		std::cerr << "bind: " << ::strerror( errno );
		std::cerr << std::endl;
		return ( EXIT_FAILURE );
	}

	// Start listening on server's address
	// through binded socket.

	//LOG( "call listen() (fd=" << this->_socket_fd << ")" );

	if ( ::listen( this->_socket_fd, 0x0 ) == -1 )
	{
		std::cerr << "listen: " << ::strerror( errno );
		std::cerr << std::endl;
		return ( EXIT_FAILURE );
	}

	this->register_read_socket();

	return ( EXIT_SUCCESS );
}
