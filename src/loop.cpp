/*                                                                            */
/* loop.cpp                                                                   */
/* mpuig-ma <mpuig-ma@student.42barcelona.com>                                */
/* Wed Feb 14 16:44:34 2024                                                   */

#include "IEvent.hpp"
#include "webserv.hpp"
#include "Log.hpp"

int
IEvent::kq = 0;

extern bool status;

void
event_loop ( int kq )
{
	int				n_events;
	struct kevent	ev;
	IEvent *        instance;

	n_events = 1;
	status = true;

	LOG( "call event_loop() (fd=" << kq << ")" );

	while ( status == true )
	{	
		// kevent() call does not return until at least one event is received
		// or when an associated timeout is exhausted.

		n_events = ::kevent( kq, 0x0, 0, &ev, 1, 0 );

		if ( status == false )
			return ;

		if ( n_events == -1 )
		{
			std::cerr << "kevent: " << ::strerror( errno ) << std::endl;
			break ; // ( EXIT_FAILURE );
		}
		
		if ( n_events == 0 )
			continue ;

		LOG( "(ev=" << ev.ident << ")" );
		
		if ( ev.flags & EVFILT_READ )
		{
			instance = static_cast< IEvent * >( ev.udata );
			instance->dispatch( ev );
		}

		// consider EVFILT_SIGNAL
	}

	LOG( "close kq (fd=" << IEvent::kq << ")" );
	close ( IEvent::kq );

	return ;
}
