/*                                                                            */
/* constructor.cpp                                                            */
/* mpuig-ma <mpuig-ma@student.42barcelona.com>                                */
/* Tue Feb 27 14:32:20 2024                                                   */

#include "Client.hpp"

Client::Client ( const Server & server_instance ): good( true ),
	_socket_fd( 0 ),
	_buffer_recv( 0x0 ),
	_buffer_send( 0x0 ),
	_server( server_instance )
{
	LOG( "call Client::Client()" );

	this->_socket_fd = ::accept( _server._socket_fd,
			(struct sockaddr *) &this->_address,
			&this->_address_len );

	if ( this->_socket_fd == -1 )
	{
		std::cerr << "client is not OK" << std::endl;
		this->good = false;
		return ;
	}

	this->_http_request = new HTTP( *this );

	this->register_recv();

	return ;
}
