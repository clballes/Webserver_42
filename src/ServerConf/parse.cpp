/*                                                                            */
/* parse.cpp                                                                  */
/* mpuig-ma <mpuig-ma@student.42barcelona.com>                                */
/* Thu Feb 15 16:50:45 2024                                                   */

#include "debug.hpp"
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
		std::cerr << ": syntax error: brackets";
		std::cerr << std::endl;
		return ( EXIT_FAILURE );
	}

	if ( count_servers( content ) < 1 )
	{
		std::cerr << PROGRAM_NAME;
		std::cerr << ": syntax error: missing server directive en el count";
		std::cerr << std::endl;
		return ( EXIT_FAILURE );
	}

	return ( EXIT_SUCCESS );
}

int
ServerConf::parse ( std::deque< std::string > & content,
	   std::deque< std::deque< std::string > > & server_blocks )
{	
	// Split into `server {}' blocks.
	ServerConf::split2blocks( content, server_blocks, "server" );

	// The following condition turns count_servers() useless
	// consider removing it ( in pre_parse )
	if ( server_blocks.size() == 0 )
	{
		std::cerr << PROGRAM_NAME;
		std::cerr << ": syntax error: missing server directive";
		std::cerr << std::endl;
		return ( EXIT_FAILURE );
	}
	
	return ( EXIT_SUCCESS );
}

// For now, nest blocks of the same category has not been tested.
// It assumes the content to iterate has been parsed previously,
// so no brackets mismatch are given.

std::deque< std::string >::iterator
fill_block( std::deque< std::string >::iterator it,
		std::deque< std::deque< std::string > > & block_list )
{
	std::deque< std::string > list;
	std::string::size_type brackets = 0;

	if ( *it == "{" )
		++it;

	while ( ! ( *it == "}" && brackets == 0 ) )
	{
		if ( *it == "{" || *it == "}" )
			brackets += ( 124 - it->at( 0 ) );

		list.push_back( *it );
		++it;
	}

	block_list.push_back( list );
	
	return ( it + 1 );
}

void
ServerConf::split2blocks( std::deque< std::string > & content,
		std::deque< std::deque< std::string > > & block_list,
		const char * block_name )
{
	std::deque< std::string >::iterator it = content.begin();

	while ( it != content.end() )
	{
		if ( it->compare( block_name ) == 0 )
		{
			it = fill_block( it + 1, block_list );
		}
		else
			++it;
	}
	
	return ;
}

int
count_brackets ( std::deque< std::string > & content )
{
	int brace;
	std::deque< std::string>::iterator it;
	
	brace = 0;
	it = content.begin();

	while ( it != content.end() )
	{
		if ( *it == "{" || *it == "}" )
			brace += ( 124 - it->at( 0x0 ) );
		++it;
	}
	return ( ( brace == 0 ) ? EXIT_SUCCESS : EXIT_FAILURE );
}

/*
 * is_regular_file
 * Returns false if `filename' is not a file.
 */

bool
is_regular_file( const std::string & filename )
{
    struct stat file_info;

	// LOG( "call isRegularFile()" )

    if ( stat( filename.c_str(), &file_info ) != 0 )
        return ( false );
    
	return ( S_ISREG( file_info.st_mode ) );
}

int
count_servers( std::deque< std::string > & content )
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
