/*                                                                            */
/* Controller.hpp                                                             */
/* mpuig-ma <mpuig-ma@student.42barcelona.com>                                */
/* Mon Mar 25 11:51:10 2024                                                   */

#ifndef _CONTROLLER_HPP_
#define _CONTROLLER_HPP_

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sys/socket.h>		/* socket,
							   accept,
							   listen,
							   send,
							   recv,
							   bind,
							   connect,
							   getaddrinfo,
							   freeaddinfo,
							   setsockopt,
							   gettsockname */
#include <sys/types.h>		/* common for
							   gai_strerror,
							   kqueue,
							   connect,
							   getaddrinfo,
							   freeaddrinfo,
							   read */
#include <netdb.h>			/* common for
							   gai_strerror,
							   getaddrinfo,
							   freeaddrinfo,
							   getprotobynumber */
#include <sys/event.h>		/* kqueue, ... */
#include <sys/time.h>		/* kqueue */

#include "string.hpp"
#include "file.hpp"
#include "Server.hpp"
#include "define.hpp"
#include "debug.hpp"

#define CONTEXT			0
#define DIRECTIVE		1
#define no	false
#define yes	true

typedef struct s_conf_opts
{
	int				type;
	const char *	identifier;
	bool			duplicate;
	const char *	nest;
	int ( *set_func )( Server &, std::string & );

}					t_conf_opts;

class Controller 
{
	public:

		Controller ( void );
		~Controller ( void );

		bool good ( void ) const;
		int load ( const char * );
		int load ( std::string & );
		int start ( void );
		int event_loop ( void );

		const std::vector< Server > & getInstances ( void ) const;
		typedef std::vector< Server >::iterator iterator;

	private:

		bool					_good;
		std::string				_buffer;
		std::vector< Server >	_instances;

		static t_conf_opts		_opts[];

		int setup_instances ( void ) ;
		void setup_defaults ( void ) ;

};

int set_allow_methods ( Server &, std::string & );
int set_autoindex ( Server &, std::string & );
int set_cgi_param ( Server &, std::string & );
int set_cgi_pass ( Server &, std::string & );
int set_client_body ( Server &, std::string & );
int set_error_page ( Server &, std::string & );
int set_index ( Server &, std::string & );
int set_listen( Server &, std::string & );
int set_root ( Server &, std::string & );
int set_server_name ( Server &, std::string & );

#endif /* !_CONTROLLER_HPP_ */
