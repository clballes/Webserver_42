/*                                                                            */
/* parse.cpp                                                                  */
/* mpuig-ma <mpuig-ma@student.42barcelona.com>                                */
/* Thu Feb 15 16:50:45 2024                                                   */

#include "Log.hpp"
#include "ServerConf.hpp"
#include <vector>
#include <deque>
#include <string>
#include "parse.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <sys/stat.h>

int
ServerConf::pre_parse ( std::deque< std::string > & content )
{
	if ( count_brackets( content ) == EXIT_FAILURE )
	{
		std::cerr << PROGRAM_NAME;
		std::cerr << ": syntax error: brackets" << std::endl;
		return ( EXIT_FAILURE );
	}
	if ( numServers( content ) <= 0 )
	{
		std::cerr << PROGRAM_NAME;
		std::cerr << ": syntax error: missing server directive" << std::endl;
		return ( EXIT_FAILURE );
	}

	return ( EXIT_SUCCESS );
}

int
count_brackets ( std::deque< std::string > & content )
{
	int brace = 0;

	for ( std::deque< std::string>::iterator it = content.begin();
			it != content.end(); ++it )
	{
		if ( brace < 0 )
			return ( EXIT_FAILURE );

		if ( *it == "{" )
			++brace;
		else if ( *it == "}" )
			--brace;
	}

	if ( brace != 0 )
		return ( EXIT_FAILURE );

	return ( EXIT_FAILURE );
}

// ----
//

bool
isRegularFile( const std::string & filename )
{
    struct stat file_info;

    if ( stat( filename.c_str(), &file_info ) != 0 )
        return ( false );

    return ( S_ISREG( file_info.st_mode ) );
}

int
numServers( std::deque< std::string > & content )
{
    int count;
   
	count = 0;
    for ( std::deque< std::string >::iterator it = content.begin();
			it != content.end(); ++it )
    {
        if ( it->find("server") != std::string::npos )
        {
            if (it->length() == 6 && (*it)[it->length()] == '\0')
                ++count;
        }
    }

    return (count);
}

/*
std::string
trim_sp( const std::string & input )
{
    std::string result = input;
    size_t lastNonSpace = result.size();

	while ( lastNonSpace > 0
			&& (result[lastNonSpace - 1] == ' '
				|| result[lastNonSpace - 1] == '\t'))
        --lastNonSpace;

    if ( lastNonSpace < result.size() )
        result.erase( lastNonSpace );

    return (result);
}
*/

std::vector< std::deque< std::string > >
splitServer( std::deque< std::string > content )
{
    int servers = numServers( content );

    std::vector<std::deque<std::string> > arrayOfLists( servers );
    std::deque<std::string>::iterator it = content.begin();
    
	while ( it != content.end() && servers!= 0 )
	{
		if ( *it == "server" )
        {
            std::deque<std::string> subList;
            arrayOfLists[servers- 1] = subList;
            ++it;
            
			while ( it != content.end() && *it != "server" ) 
                arrayOfLists[servers- 1].push_back(*it);
                ++it;
            
			if (it != content.end() && *it == "server")
                --servers;
        }
    }
    
	return (arrayOfLists);
}
