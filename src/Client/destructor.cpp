/*                                                                            */
/* destructor.cpp                                                             */
/* mpuig-ma <mpuig-ma@student.42barcelona.com>                                */
/* Tue Feb 27 14:35:59 2024                                                   */

#include "Client.hpp"

Client::~Client ( void )
{
	std::vector< Client * >::iterator it;

	LOG( "call Client::~Client()" );

	// Registered kevents to this->_socked_fd
	// are automatically deleted on close()

	if ( this->_socket_fd != 0 )
		close ( this->_socket_fd );

	for ( it = this->_server._clients.begin();
			it != this->_server._clients.end(); ++it )
	{
		if ( *it == this )
		{
			// WIP !!
			this->_buffer_send.assign( "HTTP/1.1 200 OK\r\nconnection: close\r\n" );
			::send( this->_socket_fd,
					this->_buffer_send.c_str(),
					this->_buffer_send.length(),
					0 );

			it = this->_server._clients.erase( it );
			break ;
		}
	}

	return ;
}
