/*                                                                            */
/* events.cpp                                                                 */
/* mpuig-ma <mpuig-ma@student.42barcelona.com>                                */
/* Tue Feb 27 14:36:17 2024                                                   */

#include "Client.hpp"

int
Client::accept ( int server_fd )
{
	this->_socket_fd = ::accept( server_fd,
			(struct sockaddr *) &this->_address,
			&this->_address_len );

	return ( EXIT_SUCCESS );
}

void
Client::dispatch ( void )
{
	LOG( "Client dispatch" )
	return ;
}
