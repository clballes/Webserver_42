/*                                                                            */
/* HTTP.cpp                                                                   */
/* mpuig-ma <mpuig-ma@student.42barcelona.com>                                */
/* Tue Apr  9 16:50:09 2024                                                   */

#include "HTTP.hpp"

int
HTTP::http_head ( HTTP & http )
{
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
	if ( S_ISDIR( http._request.file_info.st_mode ) )
	{
		if ( http._server.getFlag( F_AUTOINDEX, http._request.target ) )
			http._status_code = HTTP::autoindex( http );
		else
			http._status_code = FORBIDDEN;
	}
	else if ( S_ISREG( http._request.file_info.st_mode ) )
		http._status_code = load_file( http._message_body, http._request.file );
	else
		http._status_code = NOT_FOUND;
	return ( EXIT_SUCCESS );
}

int
HTTP::http_post ( HTTP & http )
{
	// regular post, not cgi, sempre torna exit success
	generate_html( http );
	http._status_code = OK;
	return ( EXIT_SUCCESS );
}

int
HTTP::http_put ( HTTP & http )
{
	std::ofstream file;

	if ( S_ISREG( http._request.file_info.st_mode )
			|| S_ISDIR( http._request.file_info.st_mode ) )
	{
		file.open( http._request.file,
				std::ofstream::out | std::ofstream::trunc | std::ofstream::binary );
		http._status_code = OK;
	}
	else
	{
		file.open( http._request.file,
				std::ofstream::out | std::ofstream::trunc | std::ofstream::binary );
		http._status_code = CREATED;
	}
	if ( http._request.body.empty() )
		http._status_code = NO_CONTENT;
	else
		file << http._request.body;
	return ( EXIT_SUCCESS );
}

int
HTTP::http_delete ( HTTP & http )
{
	if ( S_ISREG( http._request.file_info.st_mode ) )
	{
		if ( remove( http._request.target.c_str() ) == 0 )
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
