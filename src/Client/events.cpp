/*                                                                            */
/* events.cpp                                                                 */
/* mpuig-ma <mpuig-ma@student.42barcelona.com>                                */
/* Tue Feb 27 14:36:17 2024                                                   */

#include "Client.hpp"

void
Client::dispatch ( struct kevent & ev )
{
	LOG( "call Client::dispatch()" );

	if ( ev.flags & EVFILT_READ )
		this->recv_request( ev.data );
	else if ( ev.flags & EVFILT_WRITE )
		this->send_request( ev.data );
	(void) ev;

	return ;
}

int
Client::register_recv ( void )
{
	struct kevent ev;

	LOG( "call Client::register_recv()" );

	EV_SET( &ev, this->_socket_fd, EVFILT_READ,
			EV_ADD | EV_ENABLE | EV_ONESHOT, 0, 0, (void * ) this );

	if ( ::kevent( Server::kq, &ev, 1, 0x0, 0, 0 ) == -1 )		 	           // maybe move
																			   // Server::kq fd to
																			   // IEvent::kq
	{
		std::cerr << PROGRAM_NAME;
		std::cerr << ": kevent: " << ::strerror( errno );
		std::cerr << std::endl;
		return ( EXIT_FAILURE );
	}
	
	return ( EXIT_SUCCESS );
}

int
Client::register_send ( void )
{
	struct kevent ev;

	LOG( "call Client::register_send()" );

	EV_SET( &ev, this->_socket_fd, EVFILT_WRITE,
			EV_ADD | EV_ENABLE | EV_ONESHOT, 0, 0, (void *) this );

	if ( ::kevent( Server::kq, &ev, 1, 0x0, 0, 0 ) == -1 )
	{
		std::cerr << PROGRAM_NAME;
		std::cerr << " :kevent: " << ::strerror( errno );
		std::cerr << std::endl;
		return ( EXIT_FAILURE );
	}

	return ( EXIT_SUCCESS );
}

int
Client::recv_request ( int64_t data )
{
	char * buffer = new char [data];

	LOG( "call Client::recv_request()" );

	std::memset( buffer, '\0', data );
	ssize_t n = recv( this->_socket_fd, buffer, data, 0 );
	LOG ( "n: " << n );

	write( STDOUT_FILENO, buffer, data );

	return ( EXIT_SUCCESS );
}

int
Client::send_request ( int64_t data )
{
	LOG( "call Client::send_request()" );
	LOG( "data: " << data );

	return ( EXIT_SUCCESS );
}
