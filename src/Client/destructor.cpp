/*                                                                            */
/* destructor.cpp                                                             */
/* mpuig-ma <mpuig-ma@student.42barcelona.com>                                */
/* Tue Feb 27 14:35:59 2024                                                   */

#include "Client.hpp"

Client::~Client ( void )
{
	LOG( "call Client::~Client()" );

	if ( this->_http_request != 0x0 )
		delete this->_http_request;

	if ( this->_buffer_recv != 0x0 )
		delete [] this->_buffer_recv;

	if ( this->_buffer_send != 0x0 )
		delete [] this->_buffer_send;

	if ( this->_socket_fd != 0 )
		close ( this->_socket_fd );

	return ;
}
