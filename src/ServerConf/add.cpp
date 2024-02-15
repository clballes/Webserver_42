/*                                                                            */
/* add.cpp                                                                    */
/* mpuig-ma <mpuig-ma@student.42barcelona.com>                                */
/* Thu Feb 15 16:48:59 2024                                                   */

#include "Log.hpp"
#include "ServerConf.hpp"

#include <vector>
#include <deque>
#include <string>
#include "parse.hpp"

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

	// Split into `server {}' blocks.
	// call to splitServers() which may be renamed split2blocks()
	// or similar.
	// ...

	// Parse `mem' is contents.
	// do for each server {} block extracted avobe

	// Create a new instance `ServerConf'
	// for each `server {}' block.

	//ServerConf instance ( file );
	//ServerConf::instances.insert( &instance );

	return ( EXIT_SUCCESS );
}
