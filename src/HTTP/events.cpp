/*                                                                            */
/* events.cpp                                                                 */
/* mpuig-ma <mpuig-ma@student.42barcelona.com>                                */
/* Thu Feb 29 15:18:53 2024                                                   */

#include "HTTP.hpp"
#include "ft_string.h"
#include "ft_stdlib.h"

void
HTTP::dispatch ( struct kevent & ev )
{
	(void) ev;
	LOG( "call HTTP::dispatch()" );

	return ;
}

void
HTTP::perform ( void )
{
	LOG( "call HTTP::perform" );

	std::clog << "recv ";
	LOG_BUFFER( this->_client._buffer_recv.c_str() );

	// Basic composition of status_line ( response ).

	this->_client._buffer_send = ( "HTTP/1.1 xxx OK\r\n" );
	this->_client._buffer_send.append( "content-type: text/html; charset=UTF-8\r\n" );
	this->_client._buffer_send.append( "content-length: 25\r\n" );
	this->_client._buffer_send.append( "\r\n" );
	this->_client._buffer_send.append( "<html><h1>hey</h1></html>" );
	
	if ( this->parse () == EXIT_FAILURE )
		LOG( " KO" );

	this->_status_code = 200;
	this->_client._buffer_send.replace( 9, 3, ft_itoa( this->_status_code ) );

	return ;
}
