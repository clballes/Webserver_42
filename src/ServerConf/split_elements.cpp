/*                                                                            */
/* split_elements.cpp                                                         */
/* mpuig-ma <mpuig-ma@student.42barcelona.com>                                */
/* Fri Feb 16 14:39:05 2024                                                   */

#include <string>
#include <deque>
#include <iterator>
#include "parse.hpp"

static void split_line ( std::deque< std::string >::iterator,
		std::deque< std::string > & );

void
ServerConf::split_elements ( std::deque< std::string > & mem )
{
	std::reverse_iterator< std::deque< std::string >::iterator > it;
	
	LOG( "call: split_elements" )

	it = mem.rbegin();
	while ( it != mem.rend() )
	{
		split_line( ( it + 1 ).base(), mem );
		++it;
	}

	return ;
}

void
split_line ( std::deque< std::string>::iterator line,
		std::deque< std::string > & mem )
{
	std::string::size_type start, end, len;

	LOG( "=> " << *line )

	start = 0;
	while ( start < line->length() )
	{
		len = 0;
		while ( line->at( len + start ) != '\0' && line->at( len + start ) != '{' && line->at( len + start ) != '}' )
		{
			++len;
		}

		if (len == 0)
			++len;

		std::cout << "len: " << len << std::endl;
		std::cout << "\"" << line->substr( start, len ) << "\"" << std::endl;

		start += len;
	}

	(void) mem;
	(void) start;
	(void) end;
	(void) len;
	return ;
}
