/*                                                                            */
/* events.cpp                                                                 */
/* mpuig-ma <mpuig-ma@student.42barcelona.com>                                */
/* Tue Feb 27 14:36:17 2024                                                   */

#include "Client.hpp"
#include "HTTP.hpp"

void
Client::dispatch ( struct kevent & ev )
{
	LOG( "call Client::dispatch() (fd=" << ev.ident << ")" );

	if ( ev.flags & EVFILT_READ )
		this->request_recv( ev.data );
	else if ( ev.flags & EVFILT_WRITE )
		this->request_send( ev.data );

	return ;
}

int
Client::register_recv ( void )
{
	struct kevent ev;

	LOG( "call Client::register_recv() (fd=" << this->_socket_fd << ")" );

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

	LOG( "call Client::register_send() (fd=" << this->_socket_fd << ")" );

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
Client::request_recv ( int64_t data )
{
	ssize_t n;
	char * buffer = new char [data];

	LOG( "call Client::request_recv() (fd=" << this->_socket_fd << ")" );

	std::memset( buffer, '\0', data );
	n = recv( this->_socket_fd, buffer, data, 0 );
	
	if ( n == 0 )
	{
		LOG( "client closed connection (fd=" << this->_socket_fd << ")" );
		delete this;
		return ( EXIT_FAILURE );
	}

	HTTP * request = new HTTP( buffer, data );
	(void) request;

	return ( EXIT_SUCCESS );
}

int
Client::request_send ( int64_t data )
{
	LOG( "call Client::request_send() (fd=" << this->_socket_fd << ")"  );
	LOG( "data: " << data );

	return ( EXIT_SUCCESS );
}
