/*                                                                            */
/* CGI.hpp                                                                    */
/* mpuig-ma <mpuig-ma@student.42barcelona.com>                                */
/* Wed Mar 20 12:16:57 2024                                                   */



#ifndef _WEBSERV_CGI_HPP_
#define _WEBSERV_CGI_HPP_

#include "IEvent.hpp"
#include "HTTP.hpp"
// #include "Log.hpp"

class HTTP;

class CGI : public IEvent
{
	public:

		CGI ( HTTP & );
		~CGI ( void );

		void dispatch ( struct kevent & ev );
		int register_process( pid_t pid );
		int execute ( void );


	protected:
		HTTP &  _http;
		char ** _env;
		int _pipefd[2];
};

#endif /* CGI.hpp */
