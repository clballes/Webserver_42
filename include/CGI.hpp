/*                                                                            */
/* CGI.hpp                                                                    */
/* mpuig-ma <mpuig-ma@student.42barcelona.com>                                */
/* Wed Mar 20 12:16:57 2024                                                   */


#ifndef _WEBSERV_CGI_HPP_
#define _WEBSERV_CGI_HPP_

#include "IEvent.hpp"
#include "HTTP.hpp"
#include "Server.hpp"
#include <map>
#include <cstdlib>
#include <cstring>
#include <sys/stat.h>
#include <signal.h>

// #include "Log.hpp"

class HTTP;
class Server;

class CGI : public IEvent
{
	public:

		CGI ( HTTP & , Server &);
		~CGI ( void );

		void dispatch ( struct kevent & ev );
		int register_process ( void );
		int deregister_process ( void );
		int deregister_timer ( void );
		int execute ( void );
		void kill ( void );
		int parse_headers ( std::string & );
		void setmap ( void );

	protected:

		HTTP &									_http;
		Server &								_server;
		pid_t									_pid;
		std::size_t								_timer_id;
		std::map < std::string, std::string >	_envMap;
		int										_pipefd[2];
		static std::size_t						timer_id_threshold;
};

#endif /* CGI.hpp */
