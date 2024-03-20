/*                                                                            */
/* file.cpp                                                                   */
/* mpuig-ma <mpuig-ma@student.42barcelona.com>                                */
/* Tue Mar 19 16:28:32 2024                                                   */

#include "HTTP.hpp"

int
HTTP::load_file( HTTP & http, std::string & target )
{
	std::ifstream file;
	std::ifstream::pos_type pos;
	
	LOG( "call HTTP::load_file()" );

	file.open( target, std::ios::in | std::ios::binary | std::ios::ate );

	if ( file.good() == true && file.eof() == false )
	{
		// TODO: sanity checks
		
		pos = file.tellg();
		file.seekg( 0, std::ios::beg );

		http._message_body.resize( pos );
		file.read( (char *) http._message_body.data(), pos );
	}
	
	if ( file.good() == false )
	{
		LOG( "!file errored" );
		return ( FORBIDDEN );
	}

	return ( OK );
}
