/*                                                                            */
/* events.cpp                                                                 */
/* mpuig-ma <mpuig-ma@student.42barcelona.com>                                */
/* Wed Feb 14 16:46:40 2024                                                   */

#include "Server.hpp"
#include "webserv.hpp"
	
void
set_events ( int kq )
{
	struct kevent ev;
	static struct timespec ev_timeout;

	Server::const_iterator it = Server::servers.begin();

	while ( it != Server::servers.end() )
	{
		EV_SET( &ev, (*it)->socket(), EVFILT_READ, EV_ADD | EV_ENABLE | EV_CLEAR, 0, 0, &ev_timeout );

		if ( ::kevent( kq, &ev, 1, 0x0, 0, 0 ) == -1 )
		{
			std::cerr << "kevent: " << ::strerror( errno ) << std::endl;
			exit ( 0x4 );
		}

	++it;
	}

	return ;
}
