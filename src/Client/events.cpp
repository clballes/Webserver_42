/*                                                                            */
/* events.cpp                                                                 */
/* mpuig-ma <mpuig-ma@student.42barcelona.com>                                */
/* Tue Feb 27 14:36:17 2024                                                   */

#include "IEvent.hpp"
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
			EV_ADD | EV_ENABLE, 0, 0, (void * ) this );

	if ( ::kevent( IEvent::kq, &ev, 1, 0x0, 0, 0 ) == -1 )		 	           // maybe move
																			   // IEvent::kq fd to
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

	if ( ::kevent( IEvent::kq, &ev, 1, 0x0, 0, 0 ) == -1 )
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
	
	LOG( "call Client::request_recv() (fd=" << this->_socket_fd << ")" );
	
	this->_buffer_recv = new char [data];
	this->_data_recv = data;

	std::memset( this->_buffer_recv, '\0', data );
	n = recv( this->_socket_fd, this->_buffer_recv, data, 0 );
	
	if ( n == 0 )
	{
		LOG( "client closed connection (fd=" << this->_socket_fd << ")" );
		delete this;
		return ( EXIT_FAILURE );
	}

	return ( EXIT_SUCCESS );
}

int
Client::request_send ( int64_t data )
{
	LOG( "call Client::request_send() (fd=" << this->_socket_fd << ")"  );
	LOG( "data: " << data );

	return ( EXIT_SUCCESS );
}