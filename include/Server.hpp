/*                                                                            */
/* Server.hpp                                                                 */
/* mpuig-ma <mpuig-ma@student.42barcelona.com>                                */
/* Tue Feb 13 11:46:18 2024                                                   */

#ifndef _SERVER_HPP_
#define _SERVER_HPP_

#include <netinet/in.h>		/* struct sockaddr_in */
#include <arpa/inet.h>		/* ntohs, ntohl */
#include <string>
#include <vector>
#include <map>

#include "Location.hpp"
#include "define.hpp"
#include "string.hpp"
#include "log.hpp"

class Server
{
	public:

		Server ( void );
		Server& operator= ( const Server & );
		~Server ( void );
		
		bool good ( void ) const;
	
		bool hasServerName ( std::string & ) const;
		bool getFlag ( int, std::string = "" ) const;
		std::size_t getFlags ( std::string = "" ) const;
		const std::size_t & getClientMaxBodySize ( std::string = ""  ) const;
		const std::string & getCGIpass ( std::string = "" ) const;
		const std::string & getRoot ( std::string = "" ) const;
		const std::vector< std::string > & getServerNames ( void ) const;
		const std::vector< std::string > & getIndex ( std::string = "" ) const;
		const std::string & getErrorPage ( int );
		Location & getRoute ( const std::string & ) const;
		Location & getDefaultRoute ( void ) const;
		const std::string & getRouteString ( const std::string & ) const;
		const struct sockaddr_in & getListen ( void ) const;
		in_addr_t getHost ( void ) const;
		in_port_t getPort ( void ) const;
		const std::pair<int, std::string> & getRedirection ( std::string location ) const;

		int setListen( struct sockaddr_in & );
		int setFlag ( int, bool, std::string = "" );
		int setClientMaxBodySize ( const std::string &, std::string = "" );
		int setCGIpass ( const std::string &, std::string = "" );
		int setRoot ( const std::string &, std::string = "" );
		int setServerName ( const std::string & );
		int setIndex ( const std::string &, std::string = "" );
		int setErrorPage ( int, const std::string & );
		int setRoute ( const std::string & );
		int setRedirection ( const std::string &, std::string = "" );
		int setUploadDirectory ( const std::string &, std::string = "" );

		typedef std::map< std::string,
				Location, std::greater< std::string > > t_route_map;

		int check ( void );

	private:
	
		bool								_good;
		struct sockaddr_in					_address;
		std::vector< std::string >			_server_name;
		std::map< int, std::string >		_error_pages;
		t_route_map							_routes;

};

#endif /* !_SERVER_HPP_ */
