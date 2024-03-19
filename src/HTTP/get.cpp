/*                                                                            */
/* methods.cpp                                                                */
/* mpuig-ma <mpuig-ma@student.42barcelona.com>                                */
/* Mon Mar  4 15:43:13 2024                                                   */

#include "HTTP.hpp"
#include "ft_stdlib.h"
#include "parse.hpp"

/* 
 * https://datatracker.ietf.org/doc/html/rfc9110#name-methods
 */

int
HTTP::http_get ( HTTP & http )
{
	std::string target;

	LOG( "call HTTP::http_get()" );

	target.append( http._server._root );
	target.append( http._request.target );

	LOG( " target: " << target );

	// Try find `target'
	//
	// If file is not accessible
	// and autoindex is on try accessing dir.
	
	if ( is_regular_file( target ) != false )
	{
		http._message_body.append( "<html><h1>hey</h1></html>" );
		http._status_code = OK;
	}
	else if ( http._server._allow_methods & F_AUTOINDEX )
	{
		http._status_code = HTTP::autoindex( http, target );
	}
	else
		http._status_code = INTERNAL_SERVER_ERROR;

	if ( http._server._cgi_pass.length() != 0 )
	{
		LOG( " Executing Cgi" );
		//executing CGI, establishing parameters
		std::cout <<"------------------- "<< std::endl;
		char **env = init_env(http._request.method->method, target);
		executeCGI(http._server._cgi_pass, env);
		std::cout <<"-------------------- "<< std::endl;

		// CgiHandler Cgi( http );
	}
	else
	{}

	// TODO: replace to_string(); it's not c++98.

	http._response_headers["content-length"] = std::to_string( http._message_body.size() );

	return ( EXIT_SUCCESS );
}

int
HTTP::http_head ( HTTP & http )
{
	LOG( "call HTTP::http_head()" );

	if ( http._server._cgi_pass.length() != 0 )
	{}
	else
	{}

	(void) http;
	
	return ( EXIT_SUCCESS );
}
