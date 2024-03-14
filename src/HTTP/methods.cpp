/*                                                                            */
/* methods.cpp                                                                */
/* mpuig-ma <mpuig-ma@student.42barcelona.com>                                */
/* Mon Mar  4 15:43:13 2024                                                   */

#include "HTTP.hpp"
#include "ft_stdlib.h"

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

	if ( http._client._server._allow_methods & F_AUTOINDEX )
		HTTP::autoindex( http );
	else
	{}

	// WIP: solve _client._server
	if ( http._client._server._cgi_pass.length() != 0 )
	{
		std::cout << "tenim cgi en el get"<< std::endl;
		// CgiHandler Cgi;
	}
	else
	{
		std::cout << "no tenim cgi en el get"<< std::endl;
	}

	http._buffer_send.append( "HTTP/1.1 200 \r\n" );
	http._buffer_send.append( "content-type: text/html; charset=UTF-8\r\n" );
	http._buffer_send.append( "content-length: 25\r\n" );
	http._buffer_send.append( "\r\n" );
	http._buffer_send.append( "<html><h1>hey</h1></html>" );

	// Set status line
	// Compose message body

	HTTP::compose_response( http );
	
	return ( EXIT_SUCCESS );
}

int
HTTP::http_head ( HTTP & http )
{
	LOG( "call HTTP::http_head()" );

	if ( http._client._server._cgi_pass.length() != 0 )
	{
		std::cout << "tenim cgi en el post "<< std::endl;
		// CgiHandler Cgi;
	}
	else
	{
		std::cout << "no tenim cgi en el post"<< std::endl;
	}

	HTTP::compose_response( http );
	
	return ( EXIT_SUCCESS );
}

int
HTTP::http_post ( HTTP & http )
{
	LOG( "call HTTP::http_post()" );
	
	HTTP::compose_response( http );
	
	return ( EXIT_SUCCESS );
}

int
HTTP::http_put ( HTTP & http )
{
	LOG( "call HTTP::http_put()" );
	
	HTTP::compose_response( http );
	
	return ( EXIT_SUCCESS );
}
int

HTTP::http_delete ( HTTP & http )
{
	LOG( "call HTTP::http_delete()" );
	
	HTTP::compose_response( http );
	
	return ( EXIT_SUCCESS );
}

int
HTTP::compose_response ( HTTP & http )
{
	char * itoa_status = 0x0;

	LOG( "call HTTP::compose_response()" );

	/* Set status-line */

	http._status_code = 200;
	http._buffer_send.assign( "HTTP/1.1 xxx\r\n" );

	itoa_status = ft_itoa( http._status_code );
	if ( itoa_status == 0x0 )
		http._buffer_send.replace( 9, 3, "500" );
	else
		http._buffer_send.replace( 9, 3, itoa_status );
	free( itoa_status );

	/*
	for ( t_headers::iterator it = this->_headers.begin();
			it != this->_headers.end(); ++it )
		LOG( "header: " << it->first << it->second );
	*/

	// If root is not a directory, ... autoindex only works with GET
	// https://nginx.org/en/docs/http/ngx_http_autoindex_module.html

	return ( EXIT_SUCCESS );
}
