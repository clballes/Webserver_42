/*                                                                            */
/* ServerConf.cpp                                                             */
/* mpuig-ma <mpuig-ma@student.42barcelona.com>                                */
/* Wed Feb 14 17:42:22 2024                                                   */

#include "Log.hpp"
#include "ServerConf.hpp"

#include <vector>
#include <deque>
#include <string>

// We use <deque> because we don't know beforehand
// how many `server {}' there are.

std::deque< const ServerConf * >
ServerConf::instances;

int
ServerConf::add ( std::ifstream & file )
{
	// Check if file has been opened properly.

	if ( file.good() == false )
		return ( EXIT_FAILURE );

	// Store file into <deque>

	std::deque< std::string > mem;
	ServerConf::file2mem( file, mem, 0x0 );
	
	// Create a new instance  `ServerConf'
	// for each `server {}' block.

	//ServerConf instance ( file );
	//ServerConf::instances.insert( &instance );

	return ( EXIT_SUCCESS );
}

int
ServerConf::file2mem ( std::ifstream & file, std::deque< std::string > & mem,
		void ( *func )( std::string & ) )
{
	std::string line;

	while ( file.good() && ( file.rdstate() & std::ifstream::eofbit ) == 0 )
	{
		// Get next line from `file'.

		std::getline( file , line, '\n' );

		// Pass `line' to `func'(tion) in order 
		// to modify / normalize what's added to `mem'.

		if ( func != 0x0 )
			func( line );
		
		// Add (by copy) `line' to `mem'(ory).

		mem.push_back( line );
	}

	// Check if while loop ended because of file.good() being false.

	if ( file.good() == false )
		return ( EXIT_FAILURE );

	return ( EXIT_SUCCESS );
}

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

ServerConf::~ServerConf ( void )
{
	return ;
}
