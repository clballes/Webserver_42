/*                                                                            */
/* Server.cpp                                                                 */
/* mpuig-ma <mpuig-ma@student.42barcelona.com>                                */
/* Tue Feb 13 11:45:54 2024                                                   */

#include "Server.hpp"

std::set< const Server *> Server::servers;

Server::Server ( uint16_t port )
{
	::memset ( &this->_socket_fd, 0, sizeof( this->_socket_fd ) );
	this->_socket_fd = ::socket( AF_INET, SOCK_STREAM, IPPROTO_TCP );

	//::setsockopt();

	::memset( &this->_server_address, 0, sizeof( this->_server_address ) );
	this->_server_address.sin_family = AF_INET;
	this->_server_address.sin_addr.s_addr = htonl( INADDR_ANY );
	this->_server_address.sin_port = htons( port );

	LOG ( this << ": port " << port )
	LOG ( this << ": socket " << this->_socket_fd )

	if ( ::bind( this->_socket_fd, (struct sockaddr *) &this->_server_address, sizeof( this->_server_address ) ) < 0 )
	{
		std::cerr << this << ": bind " << ::strerror( errno ) << std::endl;
		::exit (0x1);
	}

	LOG ( this << ": bind OK" )

	if ( ::listen( this->_socket_fd, 0x0 ) < 0 )
	{
		std::cerr << this << ": listen " << ::strerror( errno ) << std::endl;
		::exit (0x2);
	}

	LOG ( this << ": listening" )

	// just listen
	// put event on kqueue and accept when there's activity
	//_client_address_len = sizeof( this->_client_address );
	//_client_socket_fd = ::accept( this->_socket_fd, (struct sockaddr *) &this->_client_address, &this->_client_address_len );

	::send( this->_client_socket_fd, "Hey there\n", ::strlen( "Hey there\n" ), 0 );

	(void) _client_address_len;
	(void) _client_socket_fd;
	(void) _client_address;

	return ;
}

Server::~Server ( void )
{
	if ( this->_socket_fd != 0 )
		close (this->_socket_fd);

	return ;
}

unsigned
Server::socket ( void ) const
{
	return (this->_socket_fd);
}

std::ostream &
operator << ( std::ostream & os, const Server & server )
{
	os << server._socket_fd << std::endl;

	return (os);
}
