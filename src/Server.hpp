/*                                                                            */
/* Server.hpp                                                                 */
/* clballes <clballes@student.42barcelona.com>                                */
/* Mon Jan  8 12:56:22 2024                                                   */

#ifndef _WEBSERV_SERVER_HPP_
# define _WEBSERV_SERVER_HPP_

# include "webserv.hpp"
# include <iostream>
# include <fstream>
# include <list>
# include <exception>
# include <string>
# include "webserv.hpp"

class Server
{
    public:

        Server (void);
        Server (Server&);
        Server& operator= (Server&);
        ~Server (void);

        void populateServer(std::list<std::string>listServer);

		void	setPort (int); // must use htons()
		void	setAddr (int); // rep INADDR_ANY (macro)
		int		getPort (void) const;
		int		getAddr (void) const;

    private:

		struct sockaddr_in			s_address;
		// s_address.sin_family rep AF_INET (macro)
		// s_address.sin_port rep htons(int)
		// s_address.sin_addr.s_addr rep INADDR_ANY (macro), de moment
		std::size_t					client_max_body_size; // 0 == infinit
		std::string					server_name;
        std::string					filename;
        std::list<std::string>		listServer;
        
        
        std::string listen;
        std::string root;
        std::string index;
        std::string allow_methods;

};

# endif /* Server.hpp */
