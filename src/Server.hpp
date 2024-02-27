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

		void dispatch ( void );

		bool good;

		// kqueue's file descriptor
		
		static int kq;
		static std::vector< const Server * > servers;

		static void clear ( void );

		typedef std::vector< const Server * >:: const_iterator const_iterator;
		typedef std::vector< const Server * >:: iterator iterator;

		friend std::ostream & operator << ( std::ostream&, const Server & );

// COMMENTET FOR DEBUG ONLY
//	private:

		int create_socket ( void );
		int bind_address ( void );
		int listen ( void );
		
		void register_read_socket ( void ) const;

		int accept_connection ( void );
		int receive_request ( int64_t );

		int						_socket_fd;
		struct sockaddr_in		_server_address;

		int						_client_socket_fd;
		unsigned				_client_address_len;
		struct sockaddr_in		_client_address;

};

extern bool status;

#endif /* !_SERVER_HPP_ */
