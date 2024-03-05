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
		LOG( "status code: " << this->_status_code );
		this->_client._buffer_send = strdup( "HTTP/1.1 " );
		ft_strjoin( this->_client._buffer_send, ft_itoa( this->_status_code ) );
		ft_strjoin( this->_client._buffer_send, " \r\n" );
		this->_client._data_send = strlen( this->_client._buffer_send );
	}
	else
	{
		LOG( "status code: " << this->_status_code );
		this->_client._buffer_send = strdup( "HTTP/1.1 " );
		ft_strjoin( this->_client._buffer_send, ft_itoa( this->_status_code ) );
		ft_strjoin( this->_client._buffer_send, " \r\n" );
		this->_client._data_send = strlen( this->_client._buffer_send );
	}

	write( STDOUT_FILENO,
			this->_client._buffer_recv,
			this->_client._data_recv );

	::send( this->_client._socket_fd,
			this->_client._buffer_send,
			this->_client._data_send,
			0x0 );

	return ;
}
