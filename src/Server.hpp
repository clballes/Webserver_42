/*                                                                            */
/* Server.hpp                                                                 */
/* mpuig-ma <mpuig-ma@student.42barcelona.com>                                */
/* Tue Jan  9 08:46:14 2024                                                   */

#ifndef _WEBSERV_SERVER_HPP_
# define _WEBSERV_SERVER_HPP_

# include "ServerConf.hpp"

class ServerConf;

class Server
{
	public:

		Server (void);
		Server (const Server&);
		Server (const ServerConf&);
		Server& operator= (const Server&);
		~Server (void);

	private:

		ServerConf		conf;

};

#endif /* Server.hpp */
