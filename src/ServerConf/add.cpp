/*                                                                            */
/* add.cpp                                                                    */
/* mpuig-ma <mpuig-ma@student.42barcelona.com>                                */
/* Thu Feb 15 16:48:59 2024                                                   */

#include "Log.hpp"
#include "ServerConf.hpp"

#include <deque>
#include <string>
#include "parse.hpp"

int
ServerConf::add ( std::ifstream & file )
{
	// Check if file has been properly opened.

	if ( file.good() == false )
		return ( EXIT_FAILURE );

	// Store file into `mem'

	std::deque< std::string > mem;
	if ( ServerConf::file2mem( file, mem ) == EXIT_FAILURE )
		return ( EXIT_FAILURE );
	
	// Split lines by `{}' and directives.
	ServerConf::split_elements( mem );
	
	// Normalize `mem' contents:
	// Removes comments, trims `isspace()' characters
	
	ServerConf::normalize( mem );

	// Pre-parse `mem' is contents.
	
	if ( ServerConf::pre_parse( mem ) == EXIT_FAILURE )
		return ( EXIT_FAILURE );

	// Parse `mem' is contents.

	if ( ServerConf::parse( mem ) == EXIT_FAILURE )
		return ( EXIT_FAILURE );

	//ServerConf instance ( file );
	//ServerConf::instances.insert( &instance );

	return ( EXIT_SUCCESS );
}
