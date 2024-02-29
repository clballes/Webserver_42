/*                                                                            */
/* constructor.cpp                                                            */
/* mpuig-ma <mpuig-ma@student.42barcelona.com>                                */
/* Wed Feb 28 16:18:46 2024                                                   */

#include "Log.hpp"
#include "HTTP.hpp"
#include <unistd.h>

HTTP::HTTP ( const Client & client_instance ): _client( client_instance )
{
	LOG( "call HTTP::HTTP( const Client & )" );
	
	write( STDOUT_FILENO,
			this->_client._buffer_recv,
			this->_client._data_recv );

	return ;
}
