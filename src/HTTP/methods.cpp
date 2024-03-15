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
// Reads start-line into struct s_request;
{
	LOG( "call HTTP::http_get()" );

	// WIP
	//
	// Try find request_target ( char * )
	//
	// If file is not accessible
	// and autoindex is on try accessing dir.

	// TODO: translate target += root

	if ( is_regular_file( http._request.target ) != false )
	{
		LOG( " will GET \"" << http._request.target << "\"" );

		//http._message_body.append( "content-type: text/html; charset=UTF-8\r\n" );
		//http._message_body.append( "content-length: 25\r\n" );
		//http._message_body.append( "\r\n" );
		http._message_body.append( "<html><h1>hey</h1></html>" );

	}
	else if ( http._client._server._allow_methods & F_AUTOINDEX )
		HTTP::autoindex( http );
	else
	{
		// TODO: define response _status_code
	}

	// WIP: solve _client._server
	//if ( http._client._server._cgi_pass.length() != 0 )
	//{}
	//else
	//{}

	// TODO: replace to_string(); it's not c++98.

	if ( http._message_body.size() > 0 )
		http._response_headers["content-length"] = std::to_string( http._message_body.size() );

	http._status_code = OK;

	return ( EXIT_SUCCESS );
}

int
HTTP::http_head ( HTTP & http )
{
	LOG( "call HTTP::http_head()" );

	if ( http._client._server._cgi_pass.length() != 0 )
	{}
	else
	{}

	(void) http;
	
	return ( EXIT_SUCCESS );
}

int
HTTP::http_post ( HTTP & http )
{
	LOG( "call HTTP::http_post()" );
	
	(void) http;
	
	return ( EXIT_SUCCESS );
}

int
HTTP::http_put ( HTTP & http )
{
	LOG( "call HTTP::http_put()" );

	(void) http;

	return ( EXIT_SUCCESS );
}
int

HTTP::http_delete ( HTTP & http )
{
	LOG( "call HTTP::http_delete()" );
	
	(void) http;
	
	return ( EXIT_SUCCESS );
}
