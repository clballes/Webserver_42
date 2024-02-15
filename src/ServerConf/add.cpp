/*                                                                            */
/* add.cpp                                                                    */
/* mpuig-ma <mpuig-ma@student.42barcelona.com>                                */
/* Thu Feb 15 16:48:59 2024                                                   */

#include "Log.hpp"
#include "ServerConf.hpp"

#include <vector>
#include <deque>
#include <string>

int
ServerConf::add ( std::ifstream & file )
{
	// Check if file has been opened properly.

	if ( file.good() == false )
		return ( EXIT_FAILURE );

	// Store file into `mem'

	std::deque< std::string > mem;
	if ( ServerConf::file2mem( file, mem, 0x0 ) == EXIT_FAILURE )
		return ( EXIT_FAILURE );

	// Pre-parse `mem' is contents.
	
	if ( ServerConf::pre_parse( mem ) == EXIT_FAILURE )
		return ( EXIT_FAILURE );

	// Parse `mem' is contents.

	// Split into `server {}' blocks.
	// ...

	// Create a new instance `ServerConf'
	// for each `server {}' block.

	//ServerConf instance ( file );
	//ServerConf::instances.insert( &instance );

	return ( EXIT_SUCCESS );
}
