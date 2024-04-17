/*                                                                            */
/* Server.hpp                                                                 */
/* mpuig-ma <mpuig-ma@student.42barcelona.com>                                */
/* Tue Feb 13 11:46:18 2024                                                   */

#ifndef _SERVER_HPP_
#define _SERVER_HPP_

#include <netinet/in.h>
#include <arpa/inet.h>
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

struct s_location
{
	std::size_t					flags;
	std::string					cgi_param;
	std::string					cgi_pass;
	std::string					root;
	bool						isDefault;
	std::vector< std::string >	index;

};

template <class T> struct longer : std::binary_function <T,T,bool>
{
	bool operator() (const T& x, const T& y) const
	{
		return x.length()>y.length();
	}
};

typedef struct s_location t_location;
typedef std::vector< std::string > t_vector;
typedef std::map< std::string, t_location, longer< std::string > > t_route_map;

class Server
{
	public:

		Server ( void );
		~Server ( void );
		
		bool good ( void ) const;
		void log_conf ( void );
		
		bool getFlag ( int, std::string = "" ) const;
		std::size_t getFlags ( std::string = "" ) const;
		std::size_t getClientMaxBodySize ( void ) const;
		const std::string & getCGIparam ( std::string = "" ) const ;
		const std::string & getCGIpass ( std::string = "" ) const;
		const std::string & getRoot ( std::string = "" ) const;
		t_vector::const_iterator getServerNames ( void ) const;
		t_vector::const_iterator getIndex ( std::string = "" ) const;
		const std::string & getErrorPage ( int );
		t_location & getRoute ( std::string & ) const;
		t_location & getDefaultRoute ( void ) const;
		const struct sockaddr_in & getListen ( void ) const;

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

	private:
	
		bool								_good;
		struct sockaddr_in					_address;
		std::vector< std::string >			_server_name;
		std::size_t							_client_max_body_size;
		std::map< int, std::string >		_error_pages;
		t_route_map							_routes;

};

#endif /* !_SERVER_HPP_ */
