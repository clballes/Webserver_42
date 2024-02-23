/*                                                                            */
/* normalize.cpp                                                              */
/* mpuig-ma <mpuig-ma@student.42barcelona.com>                                */
/* Fri Feb 16 13:15:29 2024                                                   */

#include "ServerConf.hpp"
#include <deque>
#include <string>
#include "parse.hpp"

void trim_comments( std::string & );
void replace_sp( std::string & str, int ( *func )( int ) );

void
ServerConf::normalize ( std::deque< std::string > & mem )
{
	std::deque< std::string >::iterator it;

	LOG( "call normalize()" )
	
	it = mem.begin();
	while ( it != mem.end() )
	{
		trim_comments( *it );
		replace_sp( *it, &std::isspace );
		trim_f( *it, &std::isspace );

		if ( it->empty() == true )
			it = mem.erase( it );
		else
			++it;
	}

	return ;
}

void
trim_comments( std::string & str )
{
	std::string::iterator it;

	LOG( "call trim_comments()" )

	it = str.begin();
	while ( it != str.end() && *it != '#' )
		++it;

	if ( it != str.end() )
		str.erase( it, str.end() );

	return ;
}

void
trim( std::string & input )
{
    std::string result;
    bool leadingSpacesOrTabs = true;

	LOG( "call trim()" )

    for ( std::size_t i = 0; i < input.length(); ++i )
	{
        if (input[i] == ' ' || input[i] == '\t')
		{
            if (!leadingSpacesOrTabs)
			{
                result += ' ';
                leadingSpacesOrTabs = true;
            }
        }
		else
		{
            result += input[i];
            leadingSpacesOrTabs = false;
        }
    }

	input = result;

	return ;
}

std::string &
trim_f( std::string & str, int ( *func )( int ) )
{
	std::string::iterator it;

	LOG( "call trim_f" )

	it = str.end() - 1;
	while ( it != str.begin() && func( *it ) )
		str.erase( it-- );

	it = str.begin();
	while ( it != str.end() && func( *it ) )
		it = str.erase ( it );
	
	return ( str );
}

void
replace_sp( std::string & str, int ( *func )( int ) )
{
	std::string::iterator it;

	LOG( "call replace_sp" )

	it = str.begin();
	while ( it != str.end() )
	{
		if ( func( *it ) != 0 )
			*it = ' ';
		++it;
	}
	
	return ;
}
