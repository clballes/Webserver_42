/*                                                                            */
/* Server.hpp                                                                 */
/* clballes <clballes@student.42barcelona.com>                                */
/* Mon Jan  8 12:56:22 2024                                                   */

#ifndef _WEBSERV_SERVER_HPP_
# define _WEBSERV_SERVER_HPP_

# include "webserv.hpp"

# include <iostream>
# include <string>
# include <list>

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
		void setAddr (uint32_t); // rep INADDR_ANY (macro)
	
		int getPort (void) const;
		void setPort (int); // must use htons()

    private:

		struct sockaddr_in		_s_address;
		std::size_t				_client_max_body_size; // 0 == infinit

		std::string				_server_name;
		std::string				filename;
		std::list<std::string>	listServer;
        
        
		std::string listen;
		std::string root;
		std::string index;
		std::string allow_methods;

};

std::ostream& operator<< (std::ostream&, const Server&);

# endif /* Server.hpp */
