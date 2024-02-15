/*                                                                            */
/* ServerConf.hpp                                                             */
/* mpuig-ma <mpuig-ma@student.42barcelona.com>                                */
/* Wed Feb 14 17:42:36 2024                                                   */

#pragma once

#include "Server.hpp"
#include <fstream>
#include <deque>

class Server;

class ServerConf
{
	public:

		friend class Server;
		static std::deque< const ServerConf * >instances;
		static int add ( std::ifstream & );
		
		typedef std::deque< const ServerConf * >::const_iterator const_iterator;
		typedef std::deque< const ServerConf * >::iterator iterator;

	private:
		
		// It does not make sense to create an empty ServerConf instance;
		// ServerConf ( void );
		ServerConf ( const ServerConf & );
		ServerConf ( const std::deque< std::string > & );
		ServerConf & operator = ( const ServerConf & );
		~ServerConf ( void );

		static int file2mem ( std::ifstream &, std::deque< std::string > &,
				void ( *func )( std::string & ) = 0x0 );
		static int pre_parse ( std::deque< std::string > & );

};
