/*                                                                            */
/* parse.cpp                                                                  */
/* mpuig-ma <mpuig-ma@student.42barcelona.com>                                */
/* Mon Mar  4 15:42:42 2024                                                   */

#include "HTTP.hpp"
#include "ft_string.h"

void
HTTP::parse ( void )
{
	LOG( "call HTTP::parse" );

	// - [ ] Read until end of header

	// - [ ] Read start-line into struct

	HTTP::start_line();

	// http_get( *this );

	return ;
}

// From RFT9112:
//
// start-line = request-line / status-line
// request-line = method SP request-target SP HTTP-version

typedef struct s_start_line
{
	int method;
} t_start_line;

int
HTTP::start_line ( void )
{
	char * method;
	int ( *func )( HTTP & );

	LOG( "call start_line()" );

	this->_request_line.method = 0x0;
	func = 0x0;

	for ( int i = 0; i < HTTP::n_methods; ++i )
	{
		method = (char *) HTTP::methods[i].method;

		// Case might be were _buffer_recv
		// has not enough bytes ... ?

		if ( ft_strncmp( method, this->_client._buffer_recv,
					ft_strlen( method ) ) == 0 )
		{
			func = HTTP::methods[i].method_func;
			break ;
		}
	}

	if ( func == 0x0 )
		LOG( NOT_IMPLEMENTED );

	//this->_request_line.method = ;
	//this->_request_line.request_target = ;
	//this->_request_line.http_version = ;

	return ( EXIT_SUCCESS );
}
