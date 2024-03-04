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
	int i;
	int count;
	char * buf;
	char * method;
	int ( *func )( HTTP & );

	LOG( "call start_line()" );

	this->_request_line.method = 0x0;
	func = 0x0;

	// Find out method
	// This will go inside another routine
	// parse_start_line_method() ?

	count = 0;
	buf = this->_client._buffer_recv;

	while ( count < this->_client._data_recv && buf[count] != SP )
		++count;

	method = ft_substr( this->_client._buffer_recv, 0, count - 1 );

	if ( count > HTTP::n_longest_method )
	{
		LOG( NOT_IMPLEMENTED );
		return ( EXIT_FAILURE );
	}

	for ( i = 0; i < HTTP::n_methods; ++i )
	{
		if ( ft_strcmp( method, HTTP::methods[i].method ) == 0 )
			break ;
	}

	if ( method != NULL )
		free( method );

	if ( i == HTTP::n_methods )
	{
		LOG( NOT_IMPLEMENTED );
		return ( EXIT_FAILURE );
	}
	else
		this->_request_line.method = HTTP::methods[i].code;

	LOG( "count: " << count );
	LOG( "data - 1: " << this->_client._data_recv - 1 );

	if ( count < this->_client._data_recv - 1 )
	{
		LOG( "error: not enough bytes..." );
		return ( EXIT_FAILURE );
	}

	buf = this->_client._buffer_recv + count;

	count = 0;
	//while ( count < this->_client._data_recv && buf[count] != SP )
	//	++count;

	//write( STDOUT_FILENO, buf, 5 );

	//this->_request_line.request_target = ;
	//this->_request_line.http_version = ;

	return ( EXIT_SUCCESS );
}
