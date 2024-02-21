/*                                                                            */
/* destructor.cpp                                                             */
/* mpuig-ma <mpuig-ma@student.42barcelona.com>                                */
/* Thu Feb 15 16:50:14 2024                                                   */

#include "Log.hpp"
#include "ServerConf.hpp"
#include <deque>

ServerConf::~ServerConf ( void )
{
	LOG( "call ~ServerConf()" )

	return ;
}

void
ServerConf::clear ( void )
{
	ServerConf::iterator it = ServerConf::instances.begin();

	while ( it != ServerConf::instances.end() )
	{
		delete ( *it );
		++it;
	}

	ServerConf::instances.clear();

	return ;
}
