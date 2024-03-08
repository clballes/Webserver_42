/*                                                                            */
/* start_line.cpp                                                             */
/* mpuig-ma <mpuig-ma@student.42barcelona.com>                                */
/* Tue Mar  5 10:37:51 2024                                                   */

#include "HTTP.hpp"
#include "ft_string.h"

// From RFT9112:
//
// start-line = request-line / status-line
// request-line = method SP request-target SP HTTP-version

int
HTTP::parse_start_line ( void )
{
	int64_t count = 0;
	char *  buf = (char *) this->_client._buffer_recv.c_str();

	LOG( "call parse_start_line()" );

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
		if ( ft_strncmp( buf,
					this->_client._buffer_recv.c_str(), count - 1) == 0 )
			break ;
	}

	if ( iterator == HTTP::n_methods )
	{
		LOG( " 501 Not implemented" );
		this->_status_code = NOT_IMPLEMENTED;
		return ( EXIT_FAILURE );
	}


	this->_request_line.method = HTTP::methods[iterator].code;
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

	*pos = count;

	return ( EXIT_SUCCESS );
}
