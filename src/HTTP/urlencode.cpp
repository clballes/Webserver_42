/*                                                                            */
/* urlencode.cpp                                                              */
/* mpuig-ma <mpuig-ma@student.42barcelona.com>                                */
/* Tue Mar 19 14:33:32 2024                                                   */

#include "HTTP.hpp"
#include "ft_stdlib.h"

std::string &
HTTP::urlencode ( std::string & url )
{
	LOG( "call HTTP::urlencode()" );

	return ( url );
}

std::string &
HTTP::urldecode ( std::string & url )
{
	std::replace(url.begin(), url.end(), '+', ' '); //aixi fa la peticio de decoded sense signes de + en el %20

	std::string::size_type pos = 0;
	std::string            num;
	int n;

	LOG( "call HTTP::urldecode()" );

	pos = url.find_first_of( "%", pos );
	while ( pos != std::string::npos )
	{
		if ( pos + 2 >= url.length() )
			LOG( "SHOULD ABORT" );

		num = url.substr( pos + 1, 2 );
		n = std::stoi( num, 0, 16 );
		url.replace( pos, 3, sizeof( char ), static_cast<char>( n ) );

		pos = url.find_first_of( "%", pos + 1 );
	}
	return ( url );
}
