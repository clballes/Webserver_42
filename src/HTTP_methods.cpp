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
	if ( can_access_file( http._request.target ) == false )
	{
		http._status_code = 404;
		http.register_send();
		return EXIT_SUCCESS;
	}
	if ( http._server.getFlag( F_AUTOINDEX, http._request.target ) == false
			&& is_regular_file( http._request.target ) == false )
	{
		http._status_code = http.check_index();
		std::cout << http._status_code << std::endl;
		if (http._status_code == 200)
			load_file( http, http._request.target );
		http.register_send();
	}
	else if ( http._server.getFlag( F_AUTOINDEX, http._request.target ) == true
			&& is_regular_file( http._request.target ) == false ) //check_regualrflie
	{
		http._status_code = autoindex( http ); //em torna 200 o 404
		http.register_send();
	}
	else if ( http._server.getCGIpass( http._request.target ).empty() )
	{
		http.cgi_ptr = new CGI( http , http._server);
		if ( http.cgi_ptr->execute() == EXIT_FAILURE )
			return ( EXIT_FAILURE );
	}
	// else
		// que fa si no fa cgi, load file noraml sense cgi 
	return ( EXIT_SUCCESS );
}

int
HTTP::http_post ( HTTP & http )
{
	DEBUG( "" );
	if ( http._server.getCGIpass( http._request.target ).empty() )
	{
		http.cgi_ptr = new CGI( http , http._server);
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
		if ( remove( http._request.target.c_str() ) == 0 ) //cehck if si 
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
