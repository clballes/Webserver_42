/*                                                                            */
/* Router.hpp                                                                 */
/* mpuig-ma <mpuig-ma@student.42barcelona.com>                                */
/* Tue Apr 16 11:52:16 2024                                                   */

#ifndef _ROUTER_HPP_
#define _ROUTER_HPP_
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
#include <vector>
#include <string>

#include "IEvent.hpp"
#include "Connection.hpp"
#include "HTTP.hpp"
#include "Server.hpp"
#include "file.hpp"
#include "string.hpp"
#include "debug.hpp"

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
	const char *	nest;
	int ( *set_func )( void *, std::string & );

}					t_conf_opts;

/*
 * class Router (TCP)
 *
 * It creates, binds, and sets to listen sockets to specific
 * struct sockaddr_in addresses.
 * Once a connection is received, routes it to its corresponding server.
 *
 */

class Router: public IEvent
{
	public:

		Router ( void );
		~Router ( void );
		
		int listen ( void );
		bool good ( void ) const;
		int load ( std::string );
		void dispatch ( struct kevent & event );

		Server * getServer ( std::string & server_name );

	private:
	
		bool						_good;
		std::vector< Connection >	_connections;
		std::vector< Server >		_servers;
		static t_conf_opts			_opts[];

		int register_read_socket ( Connection & ) const;
		int parse ( std::string & );
		int setConnection ( const struct sockaddr_in & ip_address,
				int domain = AF_INET,
				int type = SOCK_STREAM,
				int protocol = IPPROTO_TCP );
};

int set_allow_methods ( void *, std::string & );
int set_autoindex ( void *, std::string & );
int set_cgi_param ( void *, std::string & );
int set_cgi_pass ( void *, std::string & );
int set_client_body ( void *, std::string & );
int set_error_page ( void *, std::string & );
int set_index ( void *, std::string & );
int set_listen( void *, std::string & );
int set_root ( void *, std::string & );
int set_server_name ( void *, std::string & );

#endif /* !_ROUTER_HPP_ */
