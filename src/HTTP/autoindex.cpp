/*                                                                            */
/* autoindex.cpp                                                              */
/* mpuig-ma <mpuig-ma@student.42barcelona.com>                                */
/* Thu Mar 14 12:02:15 2024                                                   */

#include "HTTP.hpp"
#include "parse.hpp"
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
HTTP::autoindex ( HTTP & http )
{
	std::string page;
	std::string directory_name;
	DIR*        directory;

	// http._request.target is where the client asks
	// whilst directory_name is the translated location
	// on the server.
	LOG( "call HTTP::autoindex()"  );
	directory_name = http._request.target;

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
	page.append( http._request.target );
	page.append( "</title>" );
	// Add the style tags here
	page.append("<style>");
	page.append("img.icon {");
	page.append("    vertical-align: middle;");
	page.append("    margin-right: 8px;");
	page.append("}");
	page.append("a.link {");
	page.append("    vertical-align: middle;");
	page.append("}");
	page.append("</style>");
	page.append( "</head>" );
	page.append( "<body>" );
	page.append( "<h1>Index of /</h1>" );
	page.append( "<table>");
	page.append("<thead>" );
	page.append( "<tr class=\"header\" ><th tabindex=\"0\" role=\"button\">Name</th></tr></thead>" );
	// page.append( "<tr class=\"header\" ><th tabindex=\"0\" role=\"button\">Name</th><th  class=\"detailsColumn\" tabindex=\"0\" role=\"button\">Size</th><th class=\"detailsColumn\" tabindex=\"0\" role=\"button\">Date Modified</th></tr></thead>" );
	page.append( "<tbody>" );

	for ( struct dirent *ent = readdir( directory );
			ent != 0x0; ent = readdir( directory ) )
	{
		if (std::strcmp(ent->d_name, ".") != 0 && std::strcmp(ent->d_name, "..") != 0)
		{
			page.append("<tr>");
			page.append("<td data-value=\"");
			page.append(ent->d_name);
			page.append("\">");
			if (!is_regular_file(ent->d_name)) {
				page.append("<img src=\"/Users/clballes/Desktop/web42/tools/dir_icon.png\" alt=\"Directory Icon\" style=\"width: 16px; height: 16px; padding-inline-start: 1em; vertical-align:middle; padding-inline-end: 0.7em;\">");
			} else {
				page.append("<img src=\"/Users/clballes/Desktop/web42/tools/file_icon.png\" alt=\"File Icon\" style=\"width: 16px; height: 16px; padding-inline-start: 1em; vertical-align:middle; padding-inline-end: 0.7em;\">");
			}
			page.append("<a href=\"http://");
			page.append(http._headers["Host"]);
			if ( http._request.target.size() > 1 )
				page.append( http._request.target );
			if ( page.back() != '/' )
				page.append( "/" );
			page.append( ent->d_name );
			page.append("\" style=\"vertical-align: middle;\">"); // Apply vertical-align: middle; to the <a> tag

			// page.append( "\">" );
			page.append( ent->d_name );
			page.append( "</a>" );
			page.append("</td>");
			// page.append("<td class=\"detailsColumn\" data-value=\"0\"></td>");
			// page.append("<td class=\"detailsColumn\" data-value=\"1712583523\">4/8/24, 3:38:43 PM</td>");
			page.append("</tr>");
		}
	}
	
	// HTML end tags

	page.append( "</body>" );
	page.append( "</html>" );

    closedir( directory );

	http._message_body.append( page.c_str() );

	return ( OK );
}
