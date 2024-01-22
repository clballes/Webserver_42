/*                                                                            */
/* Server.hpp                                                                 */
/* clballes <clballes@student.42barcelona.com>                                */
/* Mon Jan  8 12:56:22 2024                                                   */

#ifndef _WEBSERV_SERVER_HPP_
# define _WEBSERV_SERVER_HPP_

# ifndef WEBSERV_OK
#  define WEBSERV_OK true
# endif /* WEBSERV_OK */

# include "webserv.hpp"

# include <iostream>
# include <string>
# include <list>

struct directives_s
{
	char* id;
};

class Server
{
	public:

		Server (void);
		Server (const Server&);
		Server& operator= (const Server&);
		~Server (void);

		void populateServer(std::list<std::string>listServer);
	
		const std::string& getName (void) const;
		void setName (const std::string&);

		const struct in_addr& getAddr (void) const;
		void setAddr (uint32_t);
	
		int getPort (void) const;
		void setPort (int);

		bool ok (void) const;
		void listen (void) const;

    private:

		bool					_status;
		struct sockaddr_in		_s_address;
		struct sockadd_in		_c_address;
		std::size_t				_client_max_body_size; // 0 == infinit

		std::string				_server_name;
		std::string				_filename;
		std::list<std::string>	_listServer;

		std::string _listen;
		std::string _root;
		std::string _index;
		std::string _allow_methods;

};

std::ostream& operator<< (std::ostream&, const Server&);

# endif /* Server.hpp */
