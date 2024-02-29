/*                                                                            */
/* HTTP.hpp                                                                   */
/* mpuig-ma <mpuig-ma@student.42barcelona.com>                                */
/* Wed Feb 28 17:46:13 2024                                                   */

#ifndef _HTTP_HPP_
#define _HTTP_HPP_

#include "IEvent.hpp"
#include "Client.hpp"
#include <string>
#include <iostream>

class Client;

class HTTP: public IEvent
{
	public:

		HTTP ( const Client & );
		~HTTP ( void );

		void dispatch ( struct kevent & );
		void perform ( void );

	private:

		const Client & _client;

		bool           _keep_alive;
		std::string    _host;
		std::string    _user_agent;

};

#endif /* !_HTTP_HPP_ */
