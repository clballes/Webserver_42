/*                                                                            */
/* Server.hpp                                                                 */
/* mpuig-ma <mpuig-ma@student.42barcelona.com>                                */
/* Tue Feb 13 11:46:18 2024                                                   */

#ifndef _SERVER_HPP_
#define _SERVER_HPP_

#include "webserv.hpp"
#include "IEvent.hpp"
#include "ServerConf.hpp"
#include <vector>

class ServerConf;

class Server: public IEvent
{
	public:

		Server ( const ServerConf & );
		~Server ( void );

		bool good;
		void dispatch ( struct kevent & );
		int start ( void );

		static int kq;     // kqueue's file descriptor
		static std::vector< Server * > servers;
		static void clear ( void );

		typedef std::vector< Server * >:: const_iterator const_iterator;
		typedef std::vector< Server * >:: iterator iterator;

		friend std::ostream & operator << ( std::ostream&, const Server & );

// COMMENTET FOR DEBUG ONLY
//	private:

		void register_read_socket ( void ) const;
		int receive_request ( int64_t );

		int						_socket_fd;
		unsigned				_address_len;
		struct sockaddr_in		_address;
		
		int _client_socket_fd;

};

extern bool status;

#endif /* !_SERVER_HPP_ */
