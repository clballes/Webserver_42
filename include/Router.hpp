/*                                                                            */
/* Router.hpp                                                                 */
/* mpuig-ma <mpuig-ma@student.42barcelona.com>                                */
/* Tue Apr 16 11:52:16 2024                                                   */

#ifndef _ROUTER_HPP_
#define _ROUTER_HPP_

#include <sys/socket.h>		// socket,
							// accept,
							// listen,
							// send,
							// recv,
							// bind,
							// connect,
							// getaddrinfo,
							// freeaddinfo,
							// setsockopt,
							// gettsockname
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
#include <stack>
#include <vector>
#include <string>
#include <sstream>
#include <cstdlib>			/* EXIT_SUCCESS, EXIT_FAILURE */
#include <cstring>			/* std::strerror */

#include "Connection.hpp"
#include "HTTP.hpp"
#include "Server.hpp"
#include "file.hpp"
#include "string.hpp"
#include "log.hpp"

#define CONTEXT			0
#define DIRECTIVE		1
#define no	false
#define yes	true

class Server;

typedef struct s_conf_opts
{
	int				type;
	const char *	identifier;
	bool			duplicate;
	bool			required;
	const char *	nest;
	int ( *set_func )( Server &, std::string &, std::string );

}					t_conf_opts;

// class Router (TCP)
//
// It creates, binds, and sets to listen sockets to specific
// struct sockaddr_in addresses.
// Once a connection is received, routes it to its corresponding server.

class Router: public IEvent
{
	public:

		Router ( void );
		~Router ( void );
		
		int listen ( void );
		bool good ( void ) const;
		int load ( std::string );
		void dispatch ( struct kevent & event );

		Connection & getConnection ( int fd );
		Server & getServer ( std::string & server_name, in_addr_t, in_port_t );
		Server & getDefaultServer ( void );

	private:
	
		bool						_good;
		std::vector< Connection >	_connections;
		std::vector< Server >		_servers;
		static t_conf_opts			_opts[];

		int register_read_socket ( Connection & ) const;
		int parse ( std::string & );
		int isConnection ( const struct sockaddr_in & ) const;
		int setConnection ( const struct sockaddr_in & ip_address,
				int domain = AF_INET,
				int type = SOCK_STREAM,
				int protocol = IPPROTO_TCP );
};

int set_limit_except ( Server &, std::string &, std::string = "" );
int set_autoindex ( Server &, std::string &, std::string = "" );
int set_cgi_param ( Server &, std::string &, std::string = "" );
int set_cgi_pass ( Server &, std::string &, std::string = "" );
int set_client_body ( Server &, std::string &, std::string = "" );
int set_error_page ( Server &, std::string &, std::string = "" );
int set_index ( Server &, std::string &, std::string = "" );
int set_listen( Server &, std::string &, std::string = "" );
int set_root ( Server &, std::string &, std::string = "" );
int set_server_name ( Server &, std::string &, std::string = "" );
int set_upload_files ( Server &, std::string &, std::string = "" );
int set_redirection ( Server &, std::string &, std::string = "" );

#endif // ! _ROUTER_HPP_
