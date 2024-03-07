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

	if ( this->parse () == EXIT_FAILURE )
	{
		LOG( "parse not OK" );
		LOG( "status code: " << this->_status_code );
	}
	
	this->_buffer_send.assign( "HTTP/1.1 400 \n\r\n" );

	::send( this->_client._socket_fd,
			this->_buffer_send.c_str(),
			this->_buffer_send.length(),
			0x0 );

	return ;
}
