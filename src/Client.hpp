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

		Client ( void );
		~Client ( void );

		int accept ( int );
		void dispatch ( void );

	private:
	
		int						_socket_fd;
		unsigned				_address_len;
		struct sockaddr_in		_address;

};

#endif /* !_CLIENT_HPP_ */
