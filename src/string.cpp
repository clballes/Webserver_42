/*                                                                            */
/* string.cpp                                                                 */
/* mpuig-ma <mpuig-ma@student.42barcelona.com>                                */
/* Wed Apr 10 14:44:58 2024                                                   */

#include "string.hpp"

std::string &
normalize ( std::string & line )
{
	line = trim_comments( line, "#" );
	line = trim_f( line, &std::isspace );
	line = replace_f( line, &std::isspace, ' ' );
	line = del_multipl_sp( line, &std::isspace );
	return ( line );
}

std::string &
trim_comments ( std::string & str, const char * comment_type )
{
	size_t pos;

	if ( comment_type == 0x0 )
		return ( str );
	pos = str.find_first_of( comment_type, 0 );
	if ( pos != std::string::npos )
		str = str.erase( pos, str.length() - pos );
	return ( str );
}

std::string &
trim_f( std::string & str, int ( *func )( int ) )
{
	std::string::iterator it;

	if ( str.empty() )
		return ( str );
	it = str.end() - 1;
	while ( it != str.begin() && func( *it ) )
		str.erase( it-- );
	it = str.begin();
	while ( it != str.end() && func( *it ) )
		it = str.erase ( it );
	return ( str );
}

std::string &
replace_f ( std::string & str, int ( *func )( int ), char c )
{
	std::string::iterator it;

	it = str.begin();
	while ( it != str.end() )
	{
		if ( func( *it ) != 0 )
			*it = c;
		++it;
	}
	return ( str );
}

std::string &
del_multipl_sp( std::string & str, int ( *func )( int ) )
{
	std::string::iterator it;
	bool prev_sp;

	it = str.begin();
	prev_sp = (bool) func( *it );
	while ( it != str.end() )
	{
		if ( prev_sp == true && func( *it ) != 0 )
		{
			it = str.erase( it );
			prev_sp = (bool) func( *it );
		}
		else
		{
			prev_sp = (bool) func( *it );
			++it;
		}
	}
	return ( str );
}

std::string &
urlencode ( std::string & url )
{
	return ( url );
}

std::string &
urldecode ( std::string & url )
{
	std::string::size_type pos = 0;
	std::string            num;
	int n;

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

std::string &
strtolower ( std::string & str )
{
	std::string::iterator it;

	it = str.begin();
	while ( it != str.end() )
	{
		if ( std::isupper( *it ) != 0 )
			*it = std::tolower( *it );
		++it;
	}
	return ( str );
}
