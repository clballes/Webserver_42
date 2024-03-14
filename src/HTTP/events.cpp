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

	if ( this->parse () == EXIT_FAILURE )
		LOG( " KO" );

	// Do HTTP method GET/POST...
	// Each method composes _buffer_send ( response ).

	if ( this->_request.method != 0x0 )
		this->_request.method->method_func( * this );
	else
		this->_buffer_send.assign( "HTTP/1.1 500\r\n\r\n" );

	return ;
}
