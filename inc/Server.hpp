/*                                                                            */
/* Server.hpp                                                                 */
/* mpuig-ma <mpuig-ma@student.42barcelona.com>                                */
/* Tue Feb 13 11:46:18 2024                                                   */

#ifndef _SERVER_HPP_
#define _SERVER_HPP_

#include <netinet/in.h>
#include <fcntl.h>
#include <unistd.h>

#include "debug.hpp"
#include "IEvent.hpp"
#include "ServerConf.hpp"
#include "HTTP.hpp"

class Server: public IEvent
{
	public:

		Server ( void );
		~Server ( void );
		
		bool good ( void ) const;
		int id ( void ) const;
		int start ( void );
		int stop ( void );
		void dispatch ( struct kevent & ev );

		ServerConf & getConf ( void );
		int getSocketFD ( void ) const;

		int setListen( struct sockaddr_in & );

	private:

		bool				_good;
		int					_socket_fd;
		unsigned			_sockaddr_len;
		struct sockaddr_in	_address;
		ServerConf			_conf;
		
		void register_read_socket ( void ) const;
		int receive_request ( int64_t );

};

#endif /* !_SERVER_HPP_ */
