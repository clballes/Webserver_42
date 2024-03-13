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

	std::clog << "recv ";
	LOG_BUFFER( this->_buffer_recv.c_str() );

	// Basic composition of status_line ( response ).
	
	if ( this->parse () == EXIT_FAILURE )
		LOG( " KO" );

	// Do HTTP method GET/POST...
	// for loop fashion

	this->compose_response();

	return ;
}

void
HTTP::compose_response ( void )
{
	char * itoa_status = 0x0;

	LOG( "call HTTP::compose_response()" );

	/* Set status-line */

	this->_status_code = 200;
	this->_buffer_send.assign( "HTTP/1.1 xxx\r\n" );

	itoa_status = ft_itoa( this->_status_code );
	if ( itoa_status == 0x0 )
		this->_buffer_send.replace( 9, 3, "500" );
	else
		this->_buffer_send.replace( 9, 3, itoa_status );
	free( itoa_status );

	/* Append headers */

	for ( t_headers::iterator it = this->_headers.begin();
			it != this->_headers.end(); ++it )
	{
		LOG( "header: " << it->first << it->second );
	}
	
	this->_buffer_send.append( "content-type: text/html; charset=UTF-8\r\n" );
	this->_buffer_send.append( "content-length: 25\r\n" );
	this->_buffer_send.append( "\r\n" );
	this->_buffer_send.append( "<html><h1>hey</h1></html>" );
	
	return ;
}
