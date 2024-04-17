/*                                                                            */
/* Connection.hpp                                                             */
/* mpuig-ma <mpuig-ma@student.42barcelona.com>                                */
/* Tue Apr 16 12:25:53 2024                                                   */

#ifndef _CONNECTION_HPP_
#define _CONNECTION_HPP_

#include <netinet/in.h>
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
#include <arpa/inet.h>
#include <fcntl.h>
#include <unistd.h>

#include "debug.hpp"

class Connection
{
	public:

		Connection& operator= ( Connection & );
		Connection ( const struct sockaddr_in &,
				int = AF_INET, int = SOCK_STREAM, int = IPPROTO_TCP );
		~Connection ( void );

		int getSocketFD ( void ) const;
		in_addr_t getHost ( void ) const;
		in_port_t getPort ( void ) const;

	private:
	
		bool						_good;
		int							_socket_fd;
		//unsigned					_sockaddr_len;
		struct sockaddr_in			_address;

};

#endif /* !_CONNECTION_HPP_ */
