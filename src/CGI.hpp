/*                                                                            */
/* CGI.hpp                                                                    */
/* mpuig-ma <mpuig-ma@student.42barcelona.com>                                */
/* Wed Mar 20 12:16:57 2024                                                   */



#ifndef _WEBSERV_CGI_HPP_
#define _WEBSERV_CGI_HPP_

#include "IEvent.hpp"
#include "HTTP.hpp"
#include "Log.hpp"

class HTTP;

class CGI : public IEvent
{
	public:

		CGI ( HTTP & );
		~CGI ( void );

		void dispatch ( struct kevent & ev );
		int register_process( void );
		int execute ( void );

		friend class HTTP;

	protected:

		HTTP &  _http;
		char ** _env;
		pid_t _cgi_pid;
};

#include "HTTP.hpp"

#endif /* CGI.hpp */