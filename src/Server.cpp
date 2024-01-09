/*                                                                            */
/* Server.cpp                                                                 */
/* mpuig-ma <mpuig-ma@student.42barcelona.com>                                */
/* Tue Jan  9 08:46:12 2024                                                   */

#include "Server.hpp"

Server::Server (void)
{
	return ;
}

Server::Server (const Server& instance)
{
	*this = instance;

	return ;
}

Server::Server (const ServerConf& instance)
{
	this->conf = instance;

	return ;
}

Server&
Server::operator= (const Server& instance)
{
	// cpy attributes
	this->conf = instance.conf;
	
	return (*this);
}

Server::~Server (void)
{
	return ;
}
