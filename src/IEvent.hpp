/*                                                                            */
/* IEvent.hpp                                                                  */
/* mpuig-ma <mpuig-ma@student.42barcelona.com>                                */
/* Tue Feb 27 12:21:48 2024                                                   */

#ifndef _IEVENT_HPP_
#define _IEVENT_HPP_

#include "webserv.hpp"

class IEvent
{
	public:
	
		virtual void dispatch ( struct kevent & ) = 0;
		virtual ~IEvent ( void ) {};

		int kq;

	private:

};

#endif /* !_IEVENT_HPP_ */
