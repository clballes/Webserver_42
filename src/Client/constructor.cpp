/*                                                                            */
/* constructor.cpp                                                            */
/* mpuig-ma <mpuig-ma@student.42barcelona.com>                                */
/* Tue Feb 27 14:32:20 2024                                                   */

#include "Client.hpp"

Client::Client ( Server & server_instance ):
	HTTP( *this ),
	good( true ),
	_socket_fd( 0 ),
	_server( server_instance )
{
	LOG( "call Client::Client()" );

	server_instance._clients.push_back( const_cast< Client * >( this ) );

	this->_socket_fd = ::accept( _server._socket_fd,
			(struct sockaddr *) &this->_address,
			&this->_address_len );

	if ( this->_socket_fd == -1 )
	{
		std::cerr << "client is not OK" << std::endl;
		this->good = false;
		return ;
	}

	this->register_recv();

	return ;
}
