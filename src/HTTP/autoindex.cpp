/*                                                                            */
/* autoindex.cpp                                                              */
/* mpuig-ma <mpuig-ma@student.42barcelona.com>                                */
/* Thu Mar 14 12:02:15 2024                                                   */

#include "HTTP.hpp"
#include <sstream>

/*
std::string
getLink ( const std::string & dirEntry, const std::string & directory_name,
		const std::string & host, int port )
{
    std::stringstream ss;

    ss << "\t\t<p><a href=\"http://" + host + ":" <<\
        port << directory_name + "/" + dirEntry + "\">" + dirEntry + "</a></p>\n";

	return ( ss.str() );
};
*/

int
HTTP::autoindex ( HTTP & http )
{
	std::string page;
	std::string directory_name;
	DIR*        directory;

	LOG( "call HTTP::autoindex()" );
	
	directory_name = http._request.target;
	LOG( " directory_name: " << directory_name );

	// TODO: translate directory using root.

	directory = opendir( directory_name.c_str() );

	if ( directory == NULL )
	{
		std::cerr << directory_name << ": " << strerror( errno );
		std::cerr << std::endl;
		return ( EXIT_FAILURE );
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
	
	page.append( "<p>" );

	/*
	if ( directory_name[0] != '/' )
		directory_name = "/" + directory_name;
	*/

	// directory != 0x0 :ASERTION
	
	/*
	for ( struct dirent *dirEntry = readdir( directory );
			dirEntry; dirEntry = readdir( directory ))
	{
		  page << getLink( std::string( dirEntry->d_name ),
				  directory_name, "host", 80 );
	}
	*/

	page.append( "</p>" );
	
	// HTML end tags

	page.append( "</body>" );
	page.append( "</html>" );

    closedir( directory );

	// TODO: add header content-length: page.length()
	// TODO: compose_message will and HTTP message ending CRLF.

	http._buffer_send.append( page.c_str() );

	return ( EXIT_SUCCESS );
}
