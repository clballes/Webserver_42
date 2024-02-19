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

void split2blocks( std::deque< std::string > &,
		std::deque< std::deque< std::string > > &,
		const char * );

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
		std::cerr << ": syntax error: missing server directive";
		std::cerr << std::endl;
		return ( EXIT_FAILURE );
	}

	return ( EXIT_SUCCESS );
}

int
ServerConf::parse ( std::deque< std::string > & content )
{
	// servers.. reallY?
	// maybe rename it hihi
	
	std::deque< std::deque< std::string > > servers;
	
	// Split into `server {}' blocks.
	// call to splitServers() which may be renamed split2blocks()
	// or similar.
	
	//ServerConf::split2blocks( mem, &servers, "server" );
	split2blocks( content, servers, "server" );

	// The following condition turns count_servers() useless
	// consider removing it ( in pre_parse )

	if ( servers.size() == 0 )
	{
		std::cerr << PROGRAM_NAME;
		std::cerr << ": syntax error: missing server directive";
		std::cerr << std::endl;
		return ( EXIT_FAILURE );
	}

	LOG( "----" )
	for ( std::deque< std::deque< std::string > >::iterator it1 = servers.begin();
			it1 != servers.end(); ++it1 )
	{
		LOG( "__ start server" )
		for ( std::deque< std::string >::iterator it2 = it1->begin();
				it2 != it1->end(); ++it2 )
		{
			LOG( "== " << *it2 )
		}
		LOG( "" )
	}
	LOG( "----" )

	// Create a new instance `ServerConf'
	// for each `server {}' block.
	// WIP
	// ...

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
			brackets += ( 0124 - it->at( 0 ) );

		list.push_back( *it );
		++it;
	}

	block_list.push_back( list );

	return ( it + 1 );
}


// adapted from splitServers

void
split2blocks( std::deque< std::string > & content,
		std::deque< std::deque< std::string > > & block_list,
		const char * block_name )
{
	std::deque< std::string >::iterator it = content.begin();

	while ( it != content.end() )
	{
		if ( it->compare( block_name ) == 0 )
			it = fill_block( it + 1, block_list );
		else
			++it;
	}

	return ;
}

int
count_brackets ( std::deque< std::string > & content )
{
	int brace = 0;
	std::deque< std::string>::iterator it = content.begin();

	while ( it != content.end() && brace > 0)
	{
		if ( *it == "{" || *it == "}" )
			brace += ( 0124 - it->at( 0x0 ) );
		++it;
	}
	
	return ( ( brace == 0 ) ? EXIT_SUCCESS : EXIT_FAILURE );
}

bool
isRegularFile( const std::string & filename )
{
    struct stat file_info;

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
