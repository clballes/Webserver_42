/*                                                                            */
/* destructor.cpp                                                             */
/* mpuig-ma <mpuig-ma@student.42barcelona.com>                                */
/* Tue Feb 27 14:35:59 2024                                                   */

#include "Client.hpp"

Client::~Client ( void )
{
	std::vector< Client * >::iterator it;

	LOG( "call Client::~Client()" );

	if ( this->_http_request != 0x0 )
		delete this->_http_request;

	if ( this->_buffer_recv != 0x0 )
		delete [] this->_buffer_recv;

	/*
	if ( this->_buffer_send != 0x0 )
		delete [] this->_buffer_send;
	*/

	// Registered kevents to this->_socked_fd
	// are automatically deleted on close()

	if ( this->_socket_fd != 0 )
		close ( this->_socket_fd );

	for ( it = this->_server._clients.begin();
			it != this->_server._clients.end(); ++it )
	{
		if ( *it == this )
		{
			it = this->_server._clients.erase( it );
			break ;
		}
	}

	return ;
}
