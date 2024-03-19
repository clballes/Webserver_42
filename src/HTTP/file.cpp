/*                                                                            */
/* file.cpp                                                                   */
/* mpuig-ma <mpuig-ma@student.42barcelona.com>                                */
/* Tue Mar 19 16:28:32 2024                                                   */

#include "HTTP.hpp"

int
HTTP::load_file( HTTP & http, std::string & target )
{
	std::ifstream file;
	std::string   line;
	
	LOG( "call HTTP::load_file()" );

	file.open( target );

	while ( file.good() == true && file.eof() == false )
	{
		file >> line;
		http._message_body.append( line );
	}

	if ( file.good() == false )
	{
		LOG( "!file errored" );
	}

	(void) http;
	(void) target;
	return ( OK );
}
