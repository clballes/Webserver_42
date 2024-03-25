/*                                                                            */
/* add.cpp                                                                    */
/* mpuig-ma <mpuig-ma@student.42barcelona.com>                                */
/* Thu Feb 15 16:48:59 2024                                                   */

#include "Log.hpp"
#include "ServerHandler.hpp"
#include <deque>
#include <string>
#include "parse.hpp"

int
ServerHandler::add ( std::ifstream & file )
{
	std::deque< std::deque< std::string > > server_blocks;
	std::deque< std::deque< std::string > >::iterator block;
	
	LOG( "call add()" )
	
	// Check if file has been properly opened. ho comprobem abans

	// if ( file.good() == false )
	// 	return ( EXIT_FAILURE );
	
	// LOG( conf_filename << ": open OK" );
	
	// Store file into `mem'

	std::deque< std::string > mem;
	if ( ServerHandler::file2mem( file, mem ) == EXIT_FAILURE )
		return ( EXIT_FAILURE );

	// Split lines by `{}' and directives.
	ServerHandler::split_elements( mem );
	
	// // Normalize `mem' contents:
	// // Removes comments, trims `isspace()' characters
	
	ServerHandler::normalize( mem );
	
	// // Pre-parse `mem' is contents.
	
	if ( ServerHandler::pre_parse( mem ) == EXIT_FAILURE )
		return ( EXIT_FAILURE );


	// // Parse `mem' is contents.
	
	if ( ServerHandler::parse( mem, server_blocks ) == EXIT_FAILURE )
		return ( EXIT_FAILURE );

	LOG( "server_blocks & creating ServerConf classes: " << server_blocks.size() );
	for(size_t i = 0; i < server_blocks.size(); i++)
	{
		ServerConf serverConf;
		if ( serverConf != EXIT_FAILURE)
		{
			_servers.push_back( serverConf );
		}
	}
	return ( EXIT_SUCCESS );
}
