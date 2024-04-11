/*                                                                            */
/* Server.hpp                                                                 */
/* mpuig-ma <mpuig-ma@student.42barcelona.com>                                */
/* Tue Feb 13 11:46:18 2024                                                   */

#ifndef _SERVER_HPP_
#define _SERVER_HPP_

#include <netinet/in.h>
#include <fcntl.h>
#include <unistd.h>
#include <string>
#include <sstream>
#include <vector>
#include <map>
#include "debug.hpp"
#include "define.hpp"
#include "IEvent.hpp"
#include "HTTP.hpp"

#ifdef ALLOW_FORBIDDEN
#include <arpa/inet.h>
#endif

struct s_location
{
	std::size_t					flags;
	std::string					cgi_param;
	std::string					cgi_pass;
	std::string					root;
	bool						isDefault;
	std::vector< std::string >	index;

};

typedef struct s_location t_location;
typedef std::vector< std::string > t_vector;

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

		int getSocketFD ( void ) const;
		bool getFlag ( int, std::string = "" ) const;
		std::size_t getFlags ( std::string = "" ) const;
		std::size_t getClientMaxBodySize ( void ) const;
		const std::string & getCGIparam ( std::string = "" ) const ;
		const std::string & getCGIpass ( std::string = "" ) const;
		const std::string & getRoot ( std::string = "" ) const;
		t_vector::const_iterator getServerNames ( void ) const;
		t_vector::const_iterator getIndex ( std::string = "" ) const;
		const std::string & getErrorPage ( int );

		int setListen( struct sockaddr_in & );
		int setFlag ( int, bool, std::string = "" );
		int setClientMaxBodySize ( std::size_t );
		int setCGIparam ( std::string &, std::string = "" );
		int setCGIpass ( std::string &, std::string = "" );
		int setRoot ( std::string &, std::string = "" );
		int setServerName ( std::string & );
		int setIndex ( std::string &, std::string = "" );
		int setErrorPage ( int, std::string & );
		std::vector< t_location >::iterator setLocation ( std::string = "" );


		void log_conf ( void );

	private:
	
		bool								_good;
		int									_socket_fd;
		unsigned							_sockaddr_len;
		struct sockaddr_in					_address;
		std::vector< std::string >			_server_name;
		std::size_t							_client_max_body_size;
		std::map< int, std::string >		_error_pages;
		std::vector< t_location >			_routes;

		void register_read_socket ( void ) const;
		const t_location & getLocation ( std::string = "" ) const;
		std::vector< t_location >::iterator findLocation ( std::string = "" );

};

#endif /* !_SERVER_HPP_ */
