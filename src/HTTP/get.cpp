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
	LOG( "call HTTP::http_get()" );

	// http._request.target.append( http._server._root );
	// http._request.target.append( http._request.target );
	LOG( " target: " << http._request.target );

	// Try find `_target'
	//
	// If file is not accessible
	// and autoindex is on try accessing dir.
	
	if ( is_regular_file( http._request.target ) == true )
	{
		http._status_code = HTTP::load_file( http, http._request.target );
	}
	else if ( http._server._flags & F_AUTOINDEX )
	{
		http._status_code = HTTP::autoindex( http, http._request.target );
	}
	else
		http._status_code = INTERNAL_SERVER_ERROR;

	if ( http._server._cgi_pass.length() != 0 )
	{
		LOG( " Executing Cgi" );
		CGI cgi( http );
		if (cgi.execute() == EXIT_FAILURE)
			return EXIT_FAILURE;
		http._status_code = 200;
	}
	else
	{
		
	}	

	// implement loop fashion
	if ( http._status_code == 404 )
	{
		LOG( http._server._error_page[404] );
		(void) HTTP::load_file( http, http._server._error_page[404] );
	}

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
