/*                                                                            */
/* loop.cpp                                                                   */
/* mpuig-ma <mpuig-ma@student.42barcelona.com>                                */
/* Wed Feb 14 16:44:34 2024                                                   */

#include "webserv.hpp"

extern bool status;

void
event_loop ( int kq )
{
	int				n_events;
	struct kevent	ev;
	IEvent *        instance;

	n_events = 1;
	status = true;

	while ( status == true )
	{
		// kevent() call does not return until at least one event is received
		// or when an associated timeout is exhausted.

		n_events = ::kevent( kq, 0x0, 0, &ev, 1, 0 );
		LOG( "n_events: " << n_events )

		if ( status == false )
			return ;

		if ( n_events == -1 )
		{
			std::cerr << "kevent: " << ::strerror( errno ) << std::endl;
			return ; // ( EXIT_FAILURE );
		}

		if ( n_events == 0 )
			continue ;

		if ( ev.flags & EVFILT_READ )
		{
			instance = static_cast< IEvent * >( ev.udata );
			instance->dispatch();
		}

		LOG( "<=" )
		// consider EVFILT_SIGNAL
	}

	return ;
}
