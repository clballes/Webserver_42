/*                                                                            */
/* autoindex.cpp                                                              */
/* mpuig-ma <mpuig-ma@student.42barcelona.com>                                */
/* Thu Mar 14 12:02:15 2024                                                   */

#include "HTTP.hpp"
#include <sstream>

/*
 * From RFC9112:
 * When making a request directly to an origin server, 
 * other than a CONNECT or server-wide OPTIONS request 
 * (as detailed below), a client MUST send only the 
 * absolute path and query components of the target URI 
 * as the request-target. If the target URI's path 
 * component is empty, the client MUST send "/" as the 
 * path within the origin-form of request-target.
 */

int
HTTP::autoindex ( HTTP & http, std::string & target )
{
	std::string page;
	std::string directory_name;
	DIR*        directory;

	LOG( "call HTTP::autoindex()" );
	
	directory_name = target;
	LOG( " directory_name: " << directory_name );

	directory = opendir( directory_name.c_str() );

	if ( directory == NULL )
	{
		std::cerr << directory_name << ": " << strerror( errno );
		std::cerr << std::endl;
		return ( NOT_FOUND );
	}

	// HTML init content tags ( html, head, title, body ).

	page.append( "<!DOCTYPE html>" );
	page.append( "<head>" );
	page.append( "<title>" );
	page.append( directory_name );
	page.append( "</title>" );
	page.append( "</head>" );
	page.append( "<body>" );
	page.append( "<h1>Index of</h1>" );

	std::string host( "localhost:8080" );

	for ( struct dirent *ent = readdir( directory );
			ent != 0x0; ent = readdir( directory ) )
	{
		// TODO: consider using table tr td format
		
		page.append( "<p>" );
		page.append( "<a href=\"" );
		page.append( "http://" );
		page.append( http._headers["Host"] );
		if ( directory_name.size() > 1 )
			page.append( directory_name );
		page.append( "/" );
		page.append( ent->d_name );
		page.append( "\">" );
		page.append( ent->d_name );
		page.append( "</a>" );
		page.append( "</p>\n" );
	}
	
	// HTML end tags

	page.append( "</body>" );
	page.append( "</html>" );

    closedir( directory );

	http._message_body.append( page.c_str() );

	return ( EXIT_SUCCESS );
}
