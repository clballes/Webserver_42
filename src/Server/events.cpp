/*                                                                            */
/* events.cpp                                                                 */
/* mpuig-ma <mpuig-ma@student.42barcelona.com>                                */
/* Wed Feb 21 15:54:38 2024                                                   */

#include "Server.hpp"

void
Server::register_read_socket ( void ) const
{
	struct kevent ev;
	//static struct timespec ev_timeout;
	
	LOG( "call register_socket() " << this->_socket_fd )

	EV_SET( &ev, this->_socket_fd, EVFILT_READ,
			EV_ADD | EV_ENABLE | EV_CLEAR, 0, 0, (void * ) this );

	if ( ::kevent( Server::kq, &ev, 1, 0x0, 0, 0 ) == -1 )
		std::cerr << "kevent: " << ::strerror( errno ) << std::endl;
	
	return ;
}

void
Server::register_ ( void ) const
{
	struct kevent ev;

	EV_SET(ev, this->_client_socket_fd, EVFILT_READ, EV_ADD, 0, 0, NULL);

	return ;
}

int
Server::accept_connection ( struct kevent * ev )
{
	LOG( "call accept_connection() " << this->_socket_fd )

	this->_client_socket_fd = accept( this->_socket_fd,
			(struct sockaddr *) &this->_client_address,
			&this->_client_address_len );
	
	if ( this->_client_socket_fd == -1 )
		std::cerr << "ee: " << strerror ( errno ) << std::endl;

	(void) ev;
	return ( EXIT_SUCCESS );
}

//char buffer[1024];
//ssize_t n = recv( serv->_socket_fd, &buffer, sizeof( buffer ), MSG_WAITALL );
//
//	if ( n == -1 )
//		std::cerr << "err: " << strerror( errno ) << std::endl;
//
//	write( STDOUT_FILENO, &buffer, n );
