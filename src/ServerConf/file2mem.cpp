/*                                                                            */
/* file2mem.cpp                                                               */
/* mpuig-ma <mpuig-ma@student.42barcelona.com>                                */
/* Thu Feb 15 16:51:17 2024                                                   */

#include "Log.hpp"
#include "ServerConf.hpp"
#include <fstream>
#include <deque>
#include <string>
#include "parse.hpp"

int
ServerConf::file2mem ( std::ifstream & file, std::deque< std::string > & mem )
{
	std::string line;

	while ( file.good() && ( file.rdstate() & std::ifstream::eofbit ) == 0 )
	{
		// Get next line from `file'.

		std::getline( file , line, '\n' );

		// If there is nothing in that line or is a comment `#',
		// skip to next iteration.

		if ( line.length() == 0 || line.at(0) == '#' )
			continue ;

		// Add (by copy) `line' to `mem'(ory).
		// In c++11 can be improved by using move copy constructor.
		
		mem.push_back( line );
	}

	// Check if while loop ended because of file.good() being false.

	if ( file.good() == false && file.eof() != true )
	{
		std::cerr << PROGRAM_NAME;
		std::cerr << ": error file2mem" << std::endl;
		return ( EXIT_FAILURE );
	}
	
	return ( EXIT_SUCCESS );
}
