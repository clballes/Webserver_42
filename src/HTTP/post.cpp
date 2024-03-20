/*                                                                            */
/* post.cpp                                                                   */
/* mpuig-ma <mpuig-ma@student.42barcelona.com>                                */
/* Tue Mar 19 12:47:34 2024                                                   */

#include "HTTP.hpp"

int
HTTP::http_post ( HTTP & http )
{
	std::string target;

	LOG( "call HTTP::http_post()" );

	target.append( http._server._root );
	target.append( http._request.target );

	LOG( " target: " << target );
	if (!target.empty()) //target carpeta tal, permisos
	{
		// http._status_code = HTTP::load_file( http, target );
		http._status_code = 200;
		std::string page;
		page.append( "<!DOCTYPE html>" );
		page.append( "<head>" );
		page.append( "<title>" );
		page.append( http._request.target );
		page.append( "</title>" );
		page.append( "<body>" );
		page.append( "POST RUNNING SUCESFULLY!! CONGRATULATIONS YOUR DATA HAS BEEN SAVED :)" );
		page.append( "</body>" );
		page.append( "</head>" );
		page.append( "</html>" );
		http._message_body.append( page.c_str() );
	}
	// Try find `target'
	//
	// If file is not accessible
	// and autoindex is on try accessing dir.
	
	// if ( is_regular_file( target ) == true )
	// {
	// 	chttp._status_code = HTTP::load_file( http, target );
	// }
	// if ( http._server._flags & F_AUTOINDEX )
	// {
	// 	http._status_code = HTTP::autoindex( http, target );
	// }
	// else
		// http._status_code = INTERNAL_SERVER_ERROR;

	if ( http._server._cgi_pass.length() != 0 )
	{
		std::cout <<"------------------- "<< std::endl;
		LOG( " Executing Cgi for post method " );
		http.execute();
		//executing CGI, establishing parameters
	}
	// else
	// {}

	// if ( http._status_code == 404 )
	// {
	// 	LOG( http._server._error_page[404] );
	// 	(void) HTTP::load_file( http, http._server._error_page[404] );
	// }
	return ( EXIT_SUCCESS );
}