/*                                                                            */
/* events.cpp                                                                 */
/* mpuig-ma <mpuig-ma@student.42barcelona.com>                                */
/* Thu Feb 29 15:18:53 2024                                                   */

#include "HTTP.hpp"

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

	this->parse ();

	write( STDOUT_FILENO,
			this->_client._buffer_recv,
			this->_client._data_recv );

	::send( this->_client._socket_fd,
			this->_client._buffer_send,
			this->_client._data_send,
			0x0 );

	return ;
}
