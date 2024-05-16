//                                                                             /
// HTTP_events.cpp                                                             /
// mpuig-ma <mpuig-ma@student.42barcelona.com>                                 /
// Thu May 16 12:30:23 2024                                                    /

#include "HTTP.hpp"

void
HTTP::dispatch ( struct kevent & ev )
{
	if ( ev.filter == EVFILT_READ && ev.flags & EV_EOF )
	{
		INFO( "Client closed connection (fd=" << this->_socket_fd << ")" );
		this->deregister_recv();
		delete this;
	}
	if ( ev.filter == EVFILT_READ )
	{
		(void) this->recv_request( ev.data );
	}
	else if ( ev.filter == EVFILT_WRITE )
	{
		(void) this->send_response();
	}
	return ;
}

int
HTTP::register_recv ( void )
{
	struct kevent ev;

	EV_SET( &ev, this->_socket_fd, EVFILT_READ,
			EV_ADD | EV_EOF | EV_ENABLE | EV_CLEAR, 0, 0, (void *) this );
	if ( ::kevent( IEvent::kq, &ev, 1, 0x0, 0, 0 ) == -1 )
	{
		ERROR( PROGRAM_NAME << ": kevent: " << std::strerror( errno ) );
		return ( EXIT_FAILURE );
	}
	return ( EXIT_SUCCESS );
}

int
HTTP::register_send ( void )
{
	struct kevent ev;

	EV_SET( &ev, this->_socket_fd, EVFILT_WRITE,
			EV_ADD | EV_ENABLE | EV_CLEAR | EV_ONESHOT, 0, 0, (void *) this );
	if ( ::kevent( IEvent::kq, &ev, 1, 0x0, 0, 0 ) == -1 )
	{
		ERROR( PROGRAM_NAME << " :kevent: " << std::strerror( errno ) );
		return ( EXIT_FAILURE );
	}
	return ( EXIT_SUCCESS );
}


int
HTTP::deregister_recv ( void )
{
	struct kevent ev;

	EV_SET( &ev, this->_socket_fd, EVFILT_READ,
			EV_DISABLE | EV_DELETE, 0, 0, 0 );
	if ( ::kevent( IEvent::kq, &ev, 1, 0x0, 0, 0 ) == -1 )
		WARN( std::strerror( errno ) );
	return ( EXIT_SUCCESS );
}
