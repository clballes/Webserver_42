/*                                                                            */
/* ServerHandler.hpp                                                          */
/* mpuig-ma <mpuig-ma@student.42barcelona.com>                                */
/* Wed Feb 28 17:46:13 2024                                                   */

#ifndef _SERVER_HANDLER_
#define _SERVER_HANDLER_

#pragma once

#	include "webserv.hpp"
#	include "ServerConf.hpp"
#	include "Log.hpp"
#	include <deque>
#	include <vector>

class ServerHandler
{
    public:
		ServerHandler ( void );
		// ServerHandler ( const ServerHandler & );
		// ServerHandler ( const std::deque< std::string > & );
		~ServerHandler ( void );

		// add calls the other function to parse the config server and create the serverConf

		static int add ( std::ifstream & );
		static int file2mem ( std::ifstream &, std::deque< std::string > & );
		static void normalize ( std::deque< std::string > & );
		static void split_elements ( std::deque< std::string > & );
		static void split2blocks( std::deque< std::string > &,
				std::deque< std::deque< std::string > > &, const char * );
		static int pre_parse ( std::deque< std::string > & );
		static int parse ( std::deque< std::string > &,
				std::deque< std::deque< std::string > > & );
			
		private:
			// storing the server config with the information
			std::vector<ServerConf> 			_servers;
};
#endif