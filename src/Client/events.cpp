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
		this->request_send();

	return ;
}

int
Client::register_recv ( void )
{
	struct kevent ev;

	LOG( "call Client::register_recv() (fd=" << this->_socket_fd << ")" );

	EV_SET( &ev, this->_socket_fd, EVFILT_READ,
			EV_ADD | EV_ENABLE | EV_CLEAR, 0, 0, (void * ) this );

	if ( ::kevent( IEvent::kq, &ev, 1, 0x0, 0, 0 ) == -1 )
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

	this->_buffer_recv.resize( data + 1 );

	n = recv( this->_socket_fd, (char *) this->_buffer_recv.data(), data, 0 );
	
	if ( n == 0 )
	{
		LOG( "client closed connection (fd=" << this->_socket_fd << ")" );
		delete this;
		return ( EXIT_FAILURE );
	}
	
	std::clog << "recv ";
	LOG_BUFFER( this->_buffer_recv.c_str() );

	// Do HTTP method
	// compose message.

	this->perform();

	// Finally send _buffer_send.
	// Consider setting send() as an event.

	this->request_send();

	return ( EXIT_SUCCESS );
}

int
Client::request_send ( void )
{
	LOG( "call Client::request_send() (fd=" << this->_socket_fd << ")"  );

	std::clog << "send ";
	LOG_BUFFER( (char *) this->_buffer_send.c_str() );

	::send( this->_socket_fd,
			this->_buffer_send.c_str(),
			this->_buffer_send.length(),
			0x0 );

	this->_buffer_send.clear();

	return ( EXIT_SUCCESS );
}
