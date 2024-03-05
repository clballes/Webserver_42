/*                                                                            */
/* Server.hpp                                                                 */
/* mpuig-ma <mpuig-ma@student.42barcelona.com>                                */
/* Tue Feb 13 11:46:18 2024                                                   */

#ifndef _SERVER_HPP_
#define _SERVER_HPP_

#include "IEvent.hpp"
#include "webserv.hpp"
#include "ServerConf.hpp"
#include "Client.hpp"
#include <vector>

class ServerConf;
class Client;

class Server: public IEvent
{
	public:

		Server ( const ServerConf & );
		~Server ( void );

		bool good;
		void dispatch ( struct kevent & );
		int start ( void );
		void stop ( void );

		static int kq;     // kqueue's file descriptor
		static std::vector< Server * > servers;
		static void clear ( void );

		typedef std::vector< Server * >:: const_iterator const_iterator;
		typedef std::vector< Server * >:: iterator iterator;

		friend class Client;
		friend std::ostream & operator << ( std::ostream &, const Server & );

	private:

		void register_read_socket ( void ) const;
		int receive_request ( int64_t );

		int						_socket_fd;
		unsigned				_address_len;
		struct sockaddr_in		_address;
		
		std::vector< Client * > _clients;

};

extern bool status;

#endif /* !_SERVER_HPP_ */
