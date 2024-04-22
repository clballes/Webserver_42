/*                                                                            */
/* HTTP.cpp                                                                   */
/* mpuig-ma <mpuig-ma@student.42barcelona.com>                                */
/* Tue Apr  9 16:50:09 2024                                                   */

#include "HTTP.hpp"
int
HTTP::http_head ( HTTP & http )
{
	DEBUG( "" );
	if ( http._server.getCGIpass( http._request.target ).length() != 0 )
	{}
	else
	{}
	(void) http;
	return ( EXIT_SUCCESS );
}

int
HTTP::http_get ( HTTP & http )
{
	DEBUG( "target: " << http._request.target );
	DEBUG( "status code: " << http._status_code );
	//mirar be si es aqesta ruta
	if ( http._server.getCGIpass( http._request.target ).empty() )
	{
		std::cout << "Entrering cgi¿?" << std::endl;
		http.cgi_ptr = new CGI( http );
		if ( http.cgi_ptr->execute() == EXIT_FAILURE )
			return ( EXIT_FAILURE );
	}
	return ( EXIT_SUCCESS );
}

int
HTTP::http_post ( HTTP & http )
{
	DEBUG( "" );
	if ( http._server.getCGIpass( http._request.target ).empty() )
	{
		http.cgi_ptr = new CGI( http );
		if ( http.cgi_ptr->execute() == EXIT_FAILURE )
			return ( EXIT_FAILURE );
	}
	return ( EXIT_SUCCESS );
}

int
HTTP::http_delete ( HTTP & http )
{
	DEBUG( "" );
	if ( is_regular_file( http._request.target ) )
	{
		if ( remove( http._request.target.c_str() ) == 0 )
		{
			http._status_code = 200;
			http._message_body.append( "<!DOCTYPE html><body><h1>File deleted.</h1></body></html>" );
		}
		else
		{
			http._status_code = 403;
			HTTP::load_file( http, http._server.getErrorPage( 403 ) );
		}
	}
	else
	{
		http._status_code = 404;
		HTTP::load_file( http, http._server.getErrorPage( 404 ) );
	}
	return ( EXIT_SUCCESS );
}
