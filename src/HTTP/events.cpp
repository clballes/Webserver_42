/*                                                                            */
/* events.cpp                                                                 */
/* mpuig-ma <mpuig-ma@student.42barcelona.com>                                */
/* Thu Feb 29 15:18:53 2024                                                   */

#include "HTTP.hpp"
#include "ft_string.h"

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

// from rft9112
// request-line   = method SP request-target SP HTTP-version

typedef struct s_start_line
{
	int method;
} t_start_line;

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

int
HTTP::http_get ( HTTP & http )
// Reads start-line into struct s_request;
{
	LOG( "call HTTP::http_get()" );

	(void) http;

	int i = 0;
	while ( i < HTTP::n_methods )
	{
		LOG( HTTP::methods[i].method );
		++i;
	}

	return ( EXIT_SUCCESS );
}

int
HTTP::http_head ( HTTP & http )
{
	LOG( "call HTTP::http_head()" )
	(void) http;

	return ( EXIT_SUCCESS );
}

int
HTTP::http_post ( HTTP & http )
{
	LOG( "call HTTP::http_post()" )
	(void) http;

	return ( EXIT_SUCCESS );
}

int
HTTP::http_put ( HTTP & http )
{
	LOG( "call HTTP::http_put()" )
	(void) http;

	return ( EXIT_SUCCESS );
}
int

HTTP::http_delete ( HTTP & http )
{
	LOG( "call HTTP::http_delete()" )
	(void) http;

	return ( EXIT_SUCCESS );
}
