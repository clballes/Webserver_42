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
	
	this->_buffer_recv = new char [data + 1];
	this->_buffer_recv[data] = '\0';
	this->_data_recv = data;

	n = recv( this->_socket_fd, this->_buffer_recv, data, 0 );

	std::cout << std::endl << this->_buffer_recv;
	
	if ( n == 0 )
	{
		LOG( "client closed connectin (fd=" << this->_socket_fd << ")" );
		delete this;
		return ( EXIT_FAILURE );
	}

	//this->register_send();
	this->_http_request->perform();
	// register HTTP.method
	// once ready should register Client::request_send
	// which in turn should send() _buffer_send

	return ( EXIT_SUCCESS );
}

int
Client::request_send ( int64_t data )
{
	LOG( "call Client::request_send() (fd=" << this->_socket_fd << ")"  );
	LOG( "data: " << data );

	this->_http_request->perform();

	return ( EXIT_SUCCESS );
}
