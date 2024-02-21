/*                                                                            */
/* events.cpp                                                                 */
/* mpuig-ma <mpuig-ma@student.42barcelona.com>                                */
/* Wed Feb 21 15:54:38 2024                                                   */

#include "Server.hpp"

void
Server::register_socket ( void ) const
{
	struct kevent ev;
	static struct timespec ev_timeout;

	EV_SET( &ev, this->_socket_fd,
			EVFILT_READ, EV_ADD | EV_ENABLE | EV_CLEAR, 0, 0,
			&ev_timeout );

	if ( ::kevent( Server::kq, &ev, 1, 0x0, 0, 0 ) == -1 )
		std::cerr << "kevent: " << ::strerror( errno ) << std::endl;
	
	return ;
}
