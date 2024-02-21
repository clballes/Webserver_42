/*                                                                            */
/* Server.hpp                                                                 */
/* mpuig-ma <mpuig-ma@student.42barcelona.com>                                */
/* Tue Feb 13 11:46:18 2024                                                   */

#ifndef _SERVER_HPP_
#define _SERVER_HPP_

#include "webserv.hpp"
#include "ServerConf.hpp"
#include <vector>

class ServerConf;

class Server
{
	public:

		Server ( const ServerConf & );
		~Server ( void );

		unsigned socket ( void ) const;

		//#ifdef DEBUG
		//void setPort();
		//void setAddr();
		//#endif
		
		static std::vector< const Server * > servers;
		typedef std::vector< const Server * >:: const_iterator const_iterator;
		typedef std::vector< const Server * >:: iterator iterator;

		friend std::ostream & operator << ( std::ostream&, const Server & );

	private:

		int create_socket ( void );
		int bind_address ( void );
		int listen ( void );

		int						_socket_fd;
		struct sockaddr_in		_server_address;

		int						_client_socket_fd;
		unsigned				_client_address_len;
		struct sockaddr_in		_client_address;

};

#endif /* !_SERVER_HPP_ */
