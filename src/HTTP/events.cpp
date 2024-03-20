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

	this->_buffer_recv.clear();

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
	
	// TODO replace to_string()

	http._buffer_send.append( std::to_string( http._status_code ) );
	http._buffer_send.append( " \r\n" );
	
	// TODO: replace to_string(); it's not c++98.
	// http._response_headers["content-type"] = "text/html"; //SUPER IMPORTANT PER FER LA RESPONSE I QUE ES PUGUI VEURE ALMENYS MISSTGES DERRORS !!!! nose mha petat abnas
	http._response_headers["content-length"] = std::to_string( http._message_body.size() );

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
	std::cout << "message body: " << http._response_headers["content-length"] << std::endl;
	http._message_body.clear();

	return ( EXIT_SUCCESS );
}
