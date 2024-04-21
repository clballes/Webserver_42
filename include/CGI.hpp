/*                                                                            */
/* CGI.hpp                                                                    */
/* mpuig-ma <mpuig-ma@student.42barcelona.com>                                */
/* Wed Mar 20 12:16:57 2024                                                   */


#ifndef _WEBSERV_CGI_HPP_
#define _WEBSERV_CGI_HPP_

#include "IEvent.hpp"
#include "HTTP.hpp"
#include <map>
#include <cstdlib>
#include <cstring>

// #include "Log.hpp"

class HTTP;

class CGI : public IEvent
{
	public:

		CGI ( HTTP & );
		~CGI ( void );

		void dispatch ( struct kevent & ev );
		int register_process( pid_t pid );
		int request_send ( void );
		int execute ( void );
		std::string getLine();
		void map_to_arr();
		void setmap();

	protected:

		HTTP &  _http;
		char ** _env;
		std::map <std::string, std::string> _envMap;
		int _pipefd[2];
		std::string _line;
};

#endif /* CGI.hpp */
