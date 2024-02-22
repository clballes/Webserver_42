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
	Server * serv;

	n_events = 1;
	status = true;

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
			exit ( 0x5 );
		}

		if ( n_events == 0 )
			continue ;

		// if ev.ident is a file descriptor from one of the servers
		// accept_connection and register read event for client incoming data.
		//
		// else if 
		// read incoming data from client.

		// Echo kevent ident which is the server's socket file descriptor.

		serv = ( Server * ) ev.udata;
		serv->accept_connection( &ev );

		// consider EVFILT_SIGNAL
	}

	return ;
}
