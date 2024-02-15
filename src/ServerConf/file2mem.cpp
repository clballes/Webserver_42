/*                                                                            */
/* file2mem.cpp                                                               */
/* mpuig-ma <mpuig-ma@student.42barcelona.com>                                */
/* Thu Feb 15 16:51:17 2024                                                   */

#include "Log.hpp"
#include "ServerConf.hpp"
#include <fstream>
#include <deque>
#include <string>

int
ServerConf::file2mem ( std::ifstream & file, std::deque< std::string > & mem,
		void ( *func )( std::string & ) )
{
	std::string line;

	while ( file.good() && ( file.rdstate() & std::ifstream::eofbit ) == 0 )
	{
		// Get next line from `file'.

		std::getline( file , line, '\n' );

		// If there is nothing in that line or is a comment `#',
		// skip to next iteration.

		if ( line.length() == 0 && line.at(0) == '#' )
			continue ;

		// Call `func'(tion) with `line'
		// in order to normalize what's added to `mem'.

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
