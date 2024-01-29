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

		const std::string& getName (void) const;
		void setName (const std::string&);

		const struct in_addr& getAddr (void) const;
		void setAddr (uint32_t);
	
		int getPort (void) const;
		void setPort (int);

		bool ok (void) const;

		void start (void);

    private:

		int _status;

		void setaddrinfo (void);
		struct addrinfo* _addrinfo_list;

		void socket (void);
		void bind (void);
		void listen (void);
		void accept (void);

		struct sockaddr_in _s_address;
		//struct sockaddr_in _c_address;
		std::size_t _client_max_body_size; // 0 == infinit

		int _socket;

		std::string _server_name;
		std::string _filename;
		std::list<std::string> _listServer;

		std::string _listen;

		std::string _listen_host;
		std::string _listen_port;

		std::string _root;
		std::string _index;
		std::string _allow_methods;

		static const struct addrinfo _hints;

};

std::ostream& operator<< (std::ostream&, const Server&);

# endif /* Server.hpp */
