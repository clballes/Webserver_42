/*                                                                            */
/* start_line.cpp                                                             */
/* mpuig-ma <mpuig-ma@student.42barcelona.com>                                */
/* Tue Mar  5 10:37:51 2024                                                   */

#include "HTTP.hpp"
#include "ft_string.h"
#include "parse.hpp"

// From RFT9112:
//
// start-line = request-line / status-line
// request-line = method SP request-target SP HTTP-version

static int parse_method ( t_request &, std::string & );
static int parse_target ( t_request &, std::string & );
static int parse_http_version ( t_request &, std::string & );

int
HTTP::parse_start_line( std::string & line )
{
	//LOG( "call HTTP::parse_start_line()" );
	//LOG_BUFFER( line.c_str() );

	this->_status_code = parse_method( this->_request, line );
	if ( this->_status_code != EXIT_SUCCESS )
		return ( EXIT_FAILURE );
	else
		line.erase( 0, line.find_first_of( SP, 0 ) + 1 );

	this->_status_code = parse_target( this->_request, line );
	if ( this->_status_code != EXIT_SUCCESS )
	{
		return ( EXIT_FAILURE );
	}
	else
		line.erase( 0, line.find_first_of( SP, 0 ) + 1 );

	this->_status_code = parse_http_version( this->_request, line );
	if ( this->_status_code != EXIT_SUCCESS )
		return ( EXIT_FAILURE );

	if ( line.back() != CR )
		return ( EXIT_FAILURE );

	return ( EXIT_SUCCESS );
}

int
parse_method( t_request & request, std::string & line )
{
	std::string::size_type  pos;
	std::string             value;
	int                     iterator;

	//LOG( "call parse_method()" );

	pos = line.find_first_of( SP, 0 );
	if ( pos == std::string::npos )
		return ( BAD_REQUEST );

	value = line.substr( 0, pos );

	if ( value.length() > HTTP::n_longest_method )
		return ( NOT_IMPLEMENTED );

	for ( iterator = 0; iterator < HTTP::n_methods; ++iterator )
	{
		if ( value.compare( HTTP::methods[iterator].method ) == 0 )
			break ;
	}
		
	if ( iterator == HTTP::n_methods )
		return ( NOT_IMPLEMENTED );

	request.method = &HTTP::methods[iterator];

	return ( EXIT_SUCCESS );
}

int
parse_target( t_request & request, std::string & line )
{
	LOG( "call parse_target()" << request.target);
	std::string::size_type  pos;

	pos = line.find_first_of( SP, 0 );
	if ( pos == std::string::npos )
		return ( BAD_REQUEST );

	HTTP::urldecode( request.target );
	std::string p_string = line.substr( 0, pos );
	if (p_string.length() < request.target.length())
	{
		std::cout << "ENTRAS QUE FAS 0" << std::endl;

		return ( FORBIDDEN );
	}
	else if (line.compare(0,request.target.length() - 1, request.target))
	{
		std::cout << "ENTRAS QUE FAS 1" << line << std::endl;
		std::string a = line.substr( request.target.length(), pos - request.target.length());
		std::cout << "line es" << line << " sub es" << " a es:" << a << std::endl;
		request.target.append( line.substr( request.target.length(), pos - request.target.length()  ));
	}
	else
	{
		std::cout << "ENTRAS QUE FAS" << std::endl;
		request.target.append( line.substr( 0, pos ));
	}
	
	LOG( " AFTER PARSING TARGET; request.target: \"" << request.target << "\"" );

	return ( EXIT_SUCCESS );
}

int
parse_http_version ( t_request & request, std::string & line )
{
	std::string::size_type  pos;
	std::string             value;

	//LOG( "call parse_http_version()" );

	pos = line.find_first_of( CR, 0 );

	if ( pos == std::string::npos )
		return ( BAD_REQUEST );

	value = line.substr( 0, pos );

	if ( value.compare( "HTTP/1.1" ) != 0 )
		return ( BAD_REQUEST );

	request.http_version = HTTP_11;

	return ( EXIT_SUCCESS );
}
