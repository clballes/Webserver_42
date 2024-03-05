/*                                                                            */
/* parse.cpp                                                                  */
/* mpuig-ma <mpuig-ma@student.42barcelona.com>                                */
/* Mon Mar  4 15:42:42 2024                                                   */

#include "HTTP.hpp"
#include "ft_string.h"

#define LF 012
#define CR 015
#define SP 040

typedef struct s_start_line
{
	int method;

} t_start_line;

int
HTTP::parse ( void )
{
	LOG( "call HTTP::parse" );

	// - [ ] Read until end of header
	// WIP

	// - [ ] Read start-line into struct

	if ( HTTP::parse_start_line() == EXIT_FAILURE )
	{
		LOG( "error HTTP::parse_start_line()" );
		return ( EXIT_FAILURE );
	}

	// http_get( *this );

	return ( EXIT_SUCCESS );
}

// From RFT9112:
//
// start-line = request-line / status-line
// request-line = method SP request-target SP HTTP-version

int
HTTP::parse_start_line ( void )
{
	int64_t count = 0;
	char *  buf = this->_client._buffer_recv;
	
	LOG( "call parse_start_line()" );
	LOG( "data_recv: " << this->_client._data_recv );

	// Implement this in a for loop fashion

	if ( HTTP::parse_method( buf, &count ) == EXIT_FAILURE )
		return ( EXIT_FAILURE );

	if ( count == this->_client._data_recv || buf[count] != SP )
	{
		LOG( " 404 Bad request" );
		this->_status_code = BAD_REQUEST;
		return ( EXIT_FAILURE );
	}
	else
		++count;

	if ( HTTP::parse_request_target( buf, &count ) == EXIT_FAILURE )
		return ( EXIT_FAILURE );
	
	if ( count == this->_client._data_recv || buf[count] != SP )
	{
		LOG( " 404 Bad request" );
		this->_status_code = BAD_REQUEST;
		return ( EXIT_FAILURE );
	}
	else
		++count;

	if ( HTTP::parse_http_version( buf, &count ) == EXIT_FAILURE )
		return ( EXIT_FAILURE );

	// Check CRLF at end
	
	if ( ( count != this->_client._data_recv && buf[count++] != CR )
			|| ( count != this->_client._data_recv && buf[count] != LF ) )
	{
		LOG( " 404 Bad request" );
		this->_status_code = BAD_REQUEST;
	}

	LOG( "OK" );

	return ( EXIT_SUCCESS );
}

int
HTTP::parse_method ( char * buf, int64_t * pos )
{
	int64_t count = *pos;
	int     iterator;

	LOG( "call HTTP::parse_method()" );

	while ( count < HTTP::n_longest_method
			&& count < this->_client._data_recv
			&& buf[count] != SP )
		++count;

	if ( count == this->_client._data_recv )
	{
		LOG( " 400 Bad request" );
		this->_status_code = BAD_REQUEST;
		return ( EXIT_FAILURE );
	}
	else if ( count == HTTP::n_longest_method )
	{
		LOG( " 501 Not implemented" );
		this->_status_code = NOT_IMPLEMENTED;
		return ( EXIT_FAILURE );
	}
	
	for ( iterator = 0; iterator < HTTP::n_methods; ++iterator )
	{
		if ( ft_strncmp( buf, this->_client._buffer_recv, count - 1) == 0 )
			break ;
	}

	if ( iterator == HTTP::n_methods )
	{
		LOG( " 501 Not implemented" );
		this->_status_code = NOT_IMPLEMENTED;
		return ( EXIT_FAILURE );
	}


	this->_request_line.method = HTTP::methods[iterator].code;
	LOG( " OK: " << this->_request_line.method );
	*pos = count;

	http_get( *this );

	return ( EXIT_SUCCESS );
}

int
HTTP::parse_request_target ( char * buf, int64_t * pos )
{
	int64_t count = *pos;

	LOG( "call HTTP::parse_request_target()" );

	while ( count < this->_client._data_recv
			&& buf[count] != SP )
	{
		++count;
	}

	this->_request_line.request_target = buf + *pos;
	write( STDOUT_FILENO, " OK: ", 5 );
	write( STDOUT_FILENO, this->_request_line.request_target, count - *pos );
	write( STDOUT_FILENO, "\n", 1 );
	*pos = count;
	
	return ( EXIT_SUCCESS );
}

int
HTTP::parse_http_version ( char * buf, int64_t * pos )
{
	int64_t count = *pos;

	LOG( "call HTTP::parse_http_version()" );

	while ( count < this->_client._data_recv
			&& buf[count] != SP && buf[count] != CR )
	{
		++count;
	}

	if ( ft_strncmp( buf + *pos, "HTTP/1.1", 8 ) != 0 )
	{
		LOG( " 400 Bad request" );
		this->_status_code = BAD_REQUEST;
		return ( EXIT_FAILURE );
	}
	else
		this->_request_line.http_version = HTTP_11;

	write( STDOUT_FILENO, " OK: ", 5 );
	write( STDOUT_FILENO, buf + *pos, count - *pos );
	write( STDOUT_FILENO, "\n", 1 );
	*pos = count;

	return ( EXIT_SUCCESS );
}
