/*                                                                            */
/* constructor.cpp                                                            */
/* mpuig-ma <mpuig-ma@student.42barcelona.com>                                */
/* Tue Feb 27 14:32:20 2024                                                   */

#include "Client.hpp"

Client::Client ( int64_t fd )
{
	// register EV_READ ( for client )
	
	struct kevent ev;

	LOG( "call Client::Client()" )

	this->_socket_fd = ::accept( server_fd,
			(struct sockaddr *) &this->_address,
			&this->_address_len );

	if ( this->_socket_fd == -1 )
	{
		std::cerr << "client is not OK" << std::endl;
	}

																	           // Use client event
																			   // instead.
																			   // register_read()	

	/*
	EV_SET( &ev, this->_socket_fd, EVFILT_READ,
			EV_ADD | EV_ENABLE | EV_ONESHOT, 0, 0, (void * ) this );

	if ( ::kevent( Server::kq, &ev, 1, 0x0, 0, 0 ) == -1 )
		std::cerr << "kevent: " << ::strerror( errno ) << std::endl;
	
	*/

	return ;
}
