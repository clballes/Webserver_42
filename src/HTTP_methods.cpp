/*                                                                            */
/* HTTP.cpp                                                                   */
/* mpuig-ma <mpuig-ma@student.42barcelona.com>                                */
/* Tue Apr  9 16:50:09 2024                                                   */

#include "HTTP.hpp"

int
HTTP::http_head ( HTTP & http )
{
	DEBUG( "target=\"" << http._request.target << "\"" );
	if ( S_ISDIR( http._request.file_info.st_mode )
			|| S_ISREG( http._request.file_info.st_mode ) )
		http._status_code = OK;
	else
		http._status_code = NOT_FOUND;
	return ( EXIT_SUCCESS );
}

int
HTTP::http_get ( HTTP & http )
{
	DEBUG( "target=\"" << http._request.file << "\"" );
	if ( S_ISDIR( http._request.file_info.st_mode ) )
	{
		if ( http._server.getFlag( F_AUTOINDEX, http._request.target ) )
			http._status_code = HTTP::autoindex( http );
		else
			http._status_code = FORBIDDEN;
	}
	else if ( S_ISREG( http._request.file_info.st_mode ) )
		http._status_code = HTTP::load_file( http, http._request.file );
	else
		http._status_code = NOT_FOUND;
	return ( EXIT_SUCCESS );
}

int
HTTP::http_post ( HTTP & http )
{
	DEBUG( "target=\"" << http._request.file << "\"" );
	// regular post, not cgi
	//if ( http._server.getCGIpass( http._request.target ).empty() )
	//{
	//	http._cgi_ptr = new CGI( http , http._server);
	//	if ( http._cgi_ptr->execute() == EXIT_FAILURE )
	//		return ( EXIT_FAILURE );
	//}
	http._status_code = OK;
	return ( EXIT_SUCCESS );
}

int
HTTP::http_delete ( HTTP & http )
{
	DEBUG( "target=\"" << http._request.file << "\"" );
	if ( S_ISREG( http._request.file_info.st_mode ) )
	{
		if ( remove( http._request.target.c_str() ) == 0 ) //cehck if si 
		{
			http._status_code = OK;
			http._message_body.append( "<!DOCTYPE html><body><h1>File deleted.</h1></body></html>" );
		}
		else
			http._status_code = FORBIDDEN;
	}
	else
		http._status_code = NOT_FOUND;
	return ( EXIT_SUCCESS );
}
