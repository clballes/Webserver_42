/*                                                                            */
/* HTTP.cpp                                                                   */
/* mpuig-ma <mpuig-ma@student.42barcelona.com>                                */
/* Tue Apr  9 16:50:09 2024                                                   */

#include "HTTP.hpp"

int
HTTP::http_head ( void * http_ptr )
{
	HTTP * http;

	http = static_cast< HTTP * >( http_ptr );
	if ( S_ISDIR( http->_request.file_info.st_mode ) )
	{
		if ( http->_server.getFlag( F_AUTOINDEX, http->_request.target ) )
			http->_response.status_code = OK;
		else
			http->_response.status_code = FORBIDDEN;
	}
	else if ( S_ISREG( http->_request.file_info.st_mode ) )
		http->_response.status_code = OK;
	else
		http->_response.status_code = NOT_FOUND;
	return ( EXIT_SUCCESS );
}

int
HTTP::http_get ( void * http_ptr )
{
	HTTP * http;

	http = static_cast< HTTP * >( http_ptr );
	if ( S_ISDIR( http->_request.file_info.st_mode ) )
	{
		if ( http->_server.getFlag( F_AUTOINDEX, http->_request.target ) )
		{
			http->_response.status_code = autoindex( http->_request,
					http->_response.body );
		}
		else
			http->_response.status_code = FORBIDDEN;
	}
	else if ( S_ISREG( http->_request.file_info.st_mode ) )
	{
		http->_response.status_code = load_file( http->_request.file,
				http->_response.body );
	}
	else
		http->_response.status_code = NOT_FOUND;
	return ( EXIT_SUCCESS );
}

int
HTTP::http_post ( void * http_ptr )
{
	HTTP * http;

	http = static_cast< HTTP * >( http_ptr );
	// regular post, not cgi, sempre torna exit success
	generate_html( http->_request, http->_response.body );
	http->_response.status_code = OK;
	return ( EXIT_SUCCESS );
}

int
HTTP::http_put ( void * http_ptr )
{
	HTTP * http;
	std::ofstream file;

	http = static_cast< HTTP * >( http_ptr );
	if ( ! http->_server.getUploadDirectory( http->_request.target ).empty() )
		http->_request.file.insert( 0, http->_server.getUploadDirectory( http->_request.target ) );
	if ( S_ISREG( http->_request.file_info.st_mode )
			|| S_ISDIR( http->_request.file_info.st_mode ) )
	{
		file.open( http->_request.file,
				std::ofstream::out | std::ofstream::trunc | std::ofstream::binary );
		http->_response.status_code = OK;
	}
	else
	{
		file.open( http->_request.file,
				std::ofstream::out | std::ofstream::trunc | std::ofstream::binary );
		http->_response.status_code = CREATED;
	}
	if ( http->_request.body.empty() )
		http->_response.status_code = NO_CONTENT;
	else
		file << http->_request.body;
	return ( EXIT_SUCCESS );
}

int
HTTP::http_delete ( void * http_ptr )
{
	HTTP * http;

	http = static_cast< HTTP * >( http_ptr );
	if ( S_ISREG( http->_request.file_info.st_mode ) )
	{
		if ( remove( http->_request.target.c_str() ) == 0 )
		{
			http->_response.status_code = OK;
			http->_response.body.append( "<!DOCTYPE html><body><h1>File deleted.</h1></body></html>" );
		}
		else
			http->_response.status_code = FORBIDDEN;
	}
	else
		http->_response.status_code = NOT_FOUND;
	return ( EXIT_SUCCESS );
}
