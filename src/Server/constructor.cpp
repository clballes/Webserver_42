/*                                                                            */
/* constructor.cpp                                                            */
/* mpuig-ma <mpuig-ma@student.42barcelona.com>                                */
/* Wed Feb 21 15:49:35 2024                                                   */

#include "Server.hpp"

int
Server::kq = 0;

// Initialize static `vector' of servers

std::vector< const Server * >
Server::servers;

Server::Server ( const ServerConf & instance ): good( true )
{
	LOG( "call Server( const ServerConf & )" )
	LOG( "> " << instance )

	//set goodbit

	std::memcpy( &this->_server_address, &instance._address,
			sizeof( instance._address ) );

	if ( Server::create_socket() == EXIT_FAILURE
		   || Server::bind_address() == EXIT_FAILURE
		   || Server::listen() == EXIT_FAILURE )
		//set badbit

	return ;
}

int
Server::create_socket ( void )
{
	LOG( "call create_socket()" )

	this->_socket_fd = ::socket( AF_INET, SOCK_STREAM, IPPROTO_TCP );

	if ( this->_socket_fd == -1 )
	{
		std::cerr << PROGRAM_NAME;
		std::cerr << ": " << ::strerror( errno );
		std::cerr << std::endl;
		// set badbit
		return ( EXIT_FAILURE );
	}

	// ::setsockopt();

	// set goodbit

	return ( EXIT_SUCCESS );
}

int
Server::bind_address ( void )
{
	LOG( "call bind_address()" )

	if ( ::bind( this->_socket_fd, (struct sockaddr *) &this->_server_address,
				sizeof( this->_server_address ) ) == -1 )
	{
		std::cerr << PROGRAM_NAME;
		std::cerr << ": " << ::strerror( errno );
		std::cerr << std::endl;
		return ( EXIT_FAILURE );
	}

	return ( EXIT_SUCCESS );
}

int
Server::listen ( void )
{
	LOG( "call listen()" )

	if ( ::listen( this->_socket_fd, 0x0 ) == - 1 )
	{
		std::cerr << PROGRAM_NAME;
		std::cerr << ": " << ::strerror( errno );
		std::cerr << std::endl;
		return ( EXIT_FAILURE );
	}

	// log server is listenning

	// just listen
	// put event on kqueue and accept when there's activity
	//_client_address_len = sizeof( this->_client_address );
	//_client_socket_fd = ::accept( this->_socket_fd,
	//		(struct sockaddr *) &this->_client_address,
	//		&this->_client_address_len );

	//::send( this->_client_socket_fd,
	//		"Hey there\n", ::strlen( "Hey there\n" ), 0 );

	(void) _client_address_len;
	(void) _client_socket_fd;
	(void) _client_address;

	return ( EXIT_SUCCESS );
}

void
Server::clear ( void )
{
	Server::iterator it = Server::servers.begin();

	for ( ; it != Server::servers.end(); ++it )
		delete *it;

	return ;
}

std::ostream &
operator << ( std::ostream & os, const Server & server )
{
	os << server._socket_fd << std::endl;

	return (os);
}
