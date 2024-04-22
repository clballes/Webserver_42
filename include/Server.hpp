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
#include "log.hpp"

template <class T> struct longer : std::binary_function <T,T,bool>
{
	bool operator() (const T& x, const T& y) const
	{
		return x.length()>y.length();
	}
};

class Server
{
	public:

		Server ( void );
		//Server ( const Server & );
		Server& operator= ( const Server & );
		~Server ( void );
		
		bool good ( void ) const;
		void log_conf ( void ) const;
	
		bool getFlag ( int, std::string = "" ) const;
		std::size_t getFlags ( std::string = "" ) const;
		std::size_t getClientMaxBodySize ( void ) const;
		const std::string & getCGIparam ( std::string = "" ) const ;
		const std::string & getCGIpass ( std::string = "" ) const;
		const std::string & getRoot ( std::string = "" ) const;
		std::vector< std::string > & getServerNames ( void ) const;
		bool hasServerName ( std::string & ) const;
		std::vector< std::string > & getIndex ( std::string = "" ) const;
		const std::string & getErrorPage ( int );
		Location & getRoute ( std::string & ) const;
		Location & getDefaultRoute ( void ) const;
		const struct sockaddr_in & getListen ( void ) const;
		in_addr_t getHost ( void ) const;
		in_port_t getPort ( void ) const;

		int setListen( struct sockaddr_in & );
		int setFlag ( int, bool, std::string = "" );
		int setClientMaxBodySize ( std::size_t );
		int setCGIparam ( std::string &, std::string = "" );
		int setCGIpass ( std::string &, std::string = "" );
		int setRoot ( std::string &, std::string = "" );
		int setServerName ( std::string & );
		int setIndex ( std::string &, std::string = "" );
		int setErrorPage ( int, std::string & );
		int setRoute ( std::string & );
		int setUploadFiles ( std::string &, std::string = "" );
		int setRedirection ( std::string &, std::string = "" );

		typedef std::map< std::string,
				Location, longer< std::string > > t_route_map;

	private:
	
		bool								_good;
		struct sockaddr_in					_address;
		std::vector< std::string >			_server_name;
		std::size_t							_client_max_body_size;
		std::map< int, std::string >		_error_pages;
		t_route_map							_routes;

};

#endif /* !_SERVER_HPP_ */
