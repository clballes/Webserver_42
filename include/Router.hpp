/*                                                                            */
/* Router.hpp                                                                 */
/* mpuig-ma <mpuig-ma@student.42barcelona.com>                                */
/* Tue Apr 16 11:52:16 2024                                                   */

#ifndef _ROUTER_HPP_
#define _ROUTER_HPP_

#include <sys/event.h>		/* kqueue, ... */
#include <sys/time.h>		/* kqueue */
#include <vector>
#include <string>

#include "IEvent.hpp"
#include "Connection.hpp"
#include "HTTP.hpp"
#include "Server.hpp"
#include "debug.hpp"

/*
 * class Router (TCP)
 *
 * It creates, binds, and sets to listen sockets to specific
 * struct sockaddr_in addresses.
 * Once a connection is received, routes it to its corresponding server.
 *
 */

class Server;

class Router: public IEvent
{
	public:

		Router ( void );
		~Router ( void );
		
		int listen ( void );
		bool good ( void ) const;
		void dispatch ( struct kevent & event );

		int setConnection ( struct sockaddr_in & ip_address,
				int domain = AF_INET,
				int type = SOCK_STREAM,
				int protocol = IPPROTO_TCP );
		int setServer ( std::vector< std::string > & server_names );

		Server * getServer ( std::string & server_name );

	private:
	
		bool						_good;
		std::vector< Connection >	_connections;
		std::vector< Server >		_servers;
		std::string					_buffer_recv;

		int register_read_socket ( Connection & ) const;
};

#endif /* !_ROUTER_HPP_ */
