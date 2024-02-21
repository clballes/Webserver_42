/*                                                                            */
/* loop.cpp                                                                   */
/* mpuig-ma <mpuig-ma@student.42barcelona.com>                                */
/* Wed Feb 14 16:44:34 2024                                                   */

#include "webserv.hpp"

void
event_loop ( int kq )
{
	int				n_events;
	bool			status = true;
	struct kevent	ev;

	n_events = 1;

	while ( status == true )
	{
		// kevent() call does not return until at least one event is received
		// or when an associated timeout is exhausted.
		n_events = ::kevent( kq, 0x0, 0, &ev, 1, 0 );

		if ( n_events == -1 )
		{
			std::cerr << "kevent: " << ::strerror( errno ) << std::endl;
			exit ( 0x5 );
		}

		if ( n_events == 0 )
			continue ;

		// Echo kevent ident which is the server's socket file descriptor.

		std::cout << "id: " << ev.ident << std::endl;
	}

	return ;
}
