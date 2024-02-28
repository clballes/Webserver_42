/*                                                                            */
/* Client.hpp                                                                 */
/* mpuig-ma <mpuig-ma@student.42barcelona.com>                                */
/* Tue Feb 27 14:32:32 2024                                                   */

#ifndef _CLIENT_HPP_
#define _CLIENT_HPP_

#include "webserv.hpp"
#include "IEvent.hpp"

class Client: public IEvent
{
	public:

		Client ( int );
		~Client ( void );

		bool good;
		void dispatch ( struct kevent & );

		int register_recv ( void );
		int register_send ( void );

		int recv_request ( int64_t );
		int send_request ( int64_t );

	private:
	
		int						_socket_fd;
		unsigned				_address_len;
		struct sockaddr_in		_address;

};

#endif /* !_CLIENT_HPP_ */
