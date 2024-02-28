/*                                                                            */
/* constructor.cpp                                                            */
/* mpuig-ma <mpuig-ma@student.42barcelona.com>                                */
/* Tue Feb 27 14:32:20 2024                                                   */

#include "Client.hpp"

Client::Client ( int server_fd ): good( true ),
	_socket_fd( 0 ), _buffer_recv( 0x0 ), _buffer_send( 0x0 )
{
	LOG( "call Client::Client()" );

	this->_socket_fd = ::accept( server_fd,
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
