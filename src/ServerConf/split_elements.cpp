/*                                                                            */
/* split_elements.cpp                                                         */
/* mpuig-ma <mpuig-ma@student.42barcelona.com>                                */
/* Fri Feb 16 14:39:05 2024                                                   */

#include "debug.hpp"
#include <string>
#include <deque>
#include <iterator>
#include "parse.hpp"
#include <algorithm>

static std::deque< std::string >::iterator split_line ( 
		std::deque< std::string >::iterator,
		std::deque< std::string > & );

void
ServerConf::split_elements ( std::deque< std::string > & mem )
{
	size_t pos, pos2;
	std::deque< std::string >::iterator it;
	
	it = mem.begin();

	while ( it != mem.end() )
	{
		pos = it->find_first_of( "{}", 0 );
		
		if ( it->find_first_of( "{}", 0 ) != std::string::npos )
		{
			pos2 = it->find("#");
			if ( pos2 != std::string::npos && pos > pos2 )
			{
				++it;
				continue ;
			}
			
			it = split_line( it, mem );
		}
		++it;
	}

	return ;
}

// renamed from addElements

std::deque< std::string >::iterator
split_line ( std::deque< std::string >::iterator it,
		std::deque< std::string > & mem )
{
	std::string::size_type start = 0, end;
	std::string line( *it );

	it = mem.erase( it ) - 1;

	end = line.find_first_of( "{}", start );
	while ( end != std::string::npos )
	{
		if ( ( end - start ) > 0 )
			it = mem.insert( ++it, line.substr( start, ( end - start ) ) );
		it = mem.insert( ++it, line.substr( end, 1 ) );

		start = end + 1;
		end = line.find_first_of( "{}", start );
	}

	if ( start < line.length() - 1)
		it = mem.insert( ++it, line.substr( start ) );

	return ( it );
}
