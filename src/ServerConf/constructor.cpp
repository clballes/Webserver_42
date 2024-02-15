/*                                                                            */
/* constructor.cpp                                                            */
/* mpuig-ma <mpuig-ma@student.42barcelona.com>                                */
/* Thu Feb 15 16:49:35 2024                                                   */

#include "Log.hpp"
#include "ServerConf.hpp"
#include <deque>

// We use <deque> because we don't know beforehand
// how many `server {}' there are.

std::deque< const ServerConf * >
ServerConf::instances;

//ServerConf::ServerConf ( void ) {};

ServerConf::ServerConf ( const ServerConf & instance )
{
	*this = instance;
	
	return ;
}

ServerConf::ServerConf ( const std::deque< std::string > & server_block )
{
	(void) server_block;

	return ;
}

ServerConf &
ServerConf::operator = ( const ServerConf & instance )
{
	(void) instance;

	return (*this);
}
