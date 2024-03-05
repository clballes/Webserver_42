/*                                                                            */
/* parse.cpp                                                                  */
/* mpuig-ma <mpuig-ma@student.42barcelona.com>                                */
/* Mon Mar  4 15:42:42 2024                                                   */

#include "HTTP.hpp"
#include "ft_string.h"

#define SP 32

typedef struct s_start_line
{
	int method;

} t_start_line;

void
HTTP::parse ( void )
{
	LOG( "call HTTP::parse" );

	// - [ ] Read until end of header
	// WIP

	// - [ ] Read start-line into struct

	if ( HTTP::parse_start_line() == EXIT_FAILURE )
		LOG( "error HTTP::parse_start_line()" );

	// http_get( *this );

	return ;
}

// From RFT9112:
//
// start-line = request-line / status-line
// request-line = method SP request-target SP HTTP-version

int
HTTP::parse_start_line ( void )
{
	int count;
	char * buf;

	count = 0;
	buf = this->_client._buffer_recv;
	
	LOG( "call parse_start_line()" );
	LOG( "data_recv: " << this->_client._data_recv );

	// Implement this in a for loop fashion

	count = HTTP::parse_method( buf );
	if ( count == -1 )
		return ( this->_status_code );
	buf += count;

	count = HTTP::parse_request_target( buf );
	if ( count == -1 )
		return ( this->_status_code );
	buf += count;

	LOG( "Seems OK to me" );
	
	return ( EXIT_SUCCESS );
}

int
HTTP::parse_method ( char * buf )
{
	int count, iterator;

	LOG( "call HTTP::parse_method()" );

	count = 0;
	LOG( " count: " << count );

	while ( count < HTTP::n_longest_method
			&& count < this->_client._data_recv
			&& buf[count] != SP )
		++count;
	
	LOG( " count: " << count );

	if ( count == HTTP::n_longest_method )
	{
		LOG( " 501 Not implemented" );
		return ( -1 );
	}
	else if ( count == this->_client._data_recv )
	{
		LOG( " 400 Bad request" );
		return ( -1 );
	}
	else
		--count;
	
	LOG( " count: " << count );

	for ( iterator = 0; iterator < HTTP::n_methods; ++iterator )
	{
		if ( ft_strncmp( buf, this->_client._buffer_recv, count ) == 0 )
			break ;
	}

	if ( iterator == HTTP::n_methods )
	{
		LOG( " 501 Not implemented" );
		return ( -1 );
	}

	LOG( " OK: " << HTTP::methods[iterator].method );

	this->_request_line.method = HTTP::methods[iterator].code;

	return ( count );
}

int
HTTP::parse_request_target ( char * buf )
{
	int count;

	count = 0;
	(void) buf;
	return ( count );
}
