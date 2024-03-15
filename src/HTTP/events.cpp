/*                                                                            */
/* events.cpp                                                                 */
/* mpuig-ma <mpuig-ma@student.42barcelona.com>                                */
/* Thu Feb 29 15:18:53 2024                                                   */

#include "HTTP.hpp"
#include "ft_string.h"
#include "ft_stdlib.h"

void
HTTP::perform ( void )
{
	LOG( "call HTTP::perform" );

	if ( this->parse() == EXIT_FAILURE )
		LOG( " KO" );

	if ( this->_request.method == 0x0 )
		this->_status_code = INTERNAL_SERVER_ERROR;
	else
	{
		// Do HTTP method GET/POST...
		// Each method sets _status_code, _headers, ...

		this->_request.method->method_func( * this );
	}

	// Generic response composition based on _status_code.

	HTTP::compose_response( *this );

	return ;
}

int
HTTP::compose_response ( HTTP & http )
{
	LOG( "call HTTP::compose_response()" );

	// status-line

	http._buffer_send.append( "HTTP/1.1 " );
	// this should be status code
	http._buffer_send.append( "200 " );
	http._buffer_send.append( "\r\n" );

	// Add response headers, if any.
	// + ending CRLF

	for ( t_headers::iterator it = http._response_headers.begin();
			it != http._response_headers.end(); ++it )
	{
		http._buffer_send.append( it->first );
		http._buffer_send.append( ": " );
		http._buffer_send.append( it->second );
		http._buffer_send.append( "\r\n" );
	}
	http._buffer_send.append( "\r\n" );

	// Add [message body], if any.

	http._buffer_send.append( http._message_body );

	return ( EXIT_SUCCESS );
}

/*
int
HTTP::compose_response ( HTTP & http )
{
	char * itoa_status = 0x0;

	LOG( "call HTTP::compose_response()" );

	// Set status-line

	http._status_code = 200;
	http._buffer_send.assign( "HTTP/1.1 xxx\r\n" );

	itoa_status = ft_itoa( http._status_code );
	if ( itoa_status == 0x0 )
		http._buffer_send.replace( 9, 3, "500" );
	else
		http._buffer_send.replace( 9, 3, itoa_status );
	free( itoa_status );

	// for ( t_headers::iterator it = this->_headers.begin();
	//		it != this->_headers.end(); ++it )
	//	LOG( "header: " << it->first << it->second );
	//

	// If root is not a directory, ... autoindex only works with GET
	// https://nginx.org/en/docs/http/ngx_http_autoindex_module.html

	return ( EXIT_SUCCESS );
}
*/
