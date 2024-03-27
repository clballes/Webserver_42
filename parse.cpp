/*                                                                            */
/* parse.cpp                                                                  */
/* mpuig-ma <mpuig-ma@student.42barcelona.com>                                */
/* Wed Mar 27 10:28:56 2024                                                   */

#include <iostream>
#include <fstream>
#include <string>
#include "debug.hpp"
#include "Log.hpp"

#define CONTEXT			0
#define DIRECTIVE		1

#define CONTEXT_MAIN	0

typedef struct s_conf_opts
{
	int          type;
	const char * identifier;
	bool         duplicate;
	const char * nest;
	int * func;

} t_conf_opts;

typedef struct s_conf_item
{
	int type;
	struct s_conf_item * next;
	struct s_conf_item * items;
	struct s_conf_item * parent;

} t_conf_item;

typedef struct s_parse
{
	int           context_lvl;
	t_conf_item   tree;

} t_parse;

void
clear_parse_tree ( t_conf_item * tree )
{
	t_conf_item * temp;

	while ( tree != NULL )
	{
		if ( tree->items != NULL )
			clear_parse_tree( tree->items );

		temp = tree;
		tree = tree->next;
		delete ( temp );
	}

	return ;
}

void
print_conf_item ( t_conf_item * tree, int offset )
{
	DEBUG ( "" );

	while ( tree != NULL )
	{
		if ( tree->type == CONTEXT && tree->items != NULL )
			print_conf_item( tree->items, offset + 2 );

		for ( int i = 0; i < offset; ++i )
			std::cout << " ";

		std::cout << "L " << tree->type << '\n';

		tree = tree->next;
	}

	return ;
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
normalize ( std::string & line )
{
	line = trim_comments( line, "#" );
	line = trim_f( line, &std::isspace );
	line = replace_f( line, &std::isspace, ' ' );

	return ( line );
}

std::string::iterator
parse_next_token ( std::string::iterator it,
		std::string::iterator end,
		t_parse * parse_obj )
{
	static std::string __context__( "main" );
	std::string::size_type size;

	while ( it != end && std::isspace( *it ) != 0 )
		++it;

	DEBUG ( *it );

	if ( *it == '{' )
	{
		std::clog << "++context\n";
		return ( ++it );
	}
	if ( *it == '}' )
	{
		std::clog << "--context\n";
		return ( ++it );
	}

	size = 0;
	while ( it != end )
	{
		if ( *it == '{' || *it == '}' )
			break ;
		if ( *it == ';' )
		{
			++it;
			break ;
		}
		++it;
		++size;
	}

	(void) parse_obj;
	return ( it );
}

std::size_t
next_token ( std::string & buffer, std::size_t pos, std::string & token )
{
	std::size_t len, it;

	len = 0;
	it = pos;
	while ( it != buffer.length() )
	{
		if ( buffer[it] == '{' || buffer[it] == '}' )
			break ;
		if ( buffer[it] == ';' )
		{
			++len;
			break ;
		}
		++len;
		++it;
	}
	token.assign ( buffer.substr( pos, len == 0 ? 1 : len ) );
	return ( len == 0 ? 1 : len );
}

int
parse ( std::string & buffer, t_parse * parse_obj )
{
	std::size_t				pos;
	std::string				token;
	const t_conf_opts		opts[] =
	{
		{ CONTEXT, "http", false, "main", 0x0 },
		{ CONTEXT, "server", true, "http", 0x0 },
		{ CONTEXT, "location", true, "server,location", 0x0 },
		{ DIRECTIVE, "server_name", false, "server", 0x0 },
		{ 0, 0x0, 0, 0x0, 0x0 }
	};

	DEBUG( buffer.c_str() );
	parse_obj->tree.type = CONTEXT;
	
	pos = 0;
	while ( pos != buffer.length() )
	{
		pos += next_token( buffer, pos, token );
		DEBUG( token.c_str() );
	}

	(void) opts;
	return ( EXIT_SUCCESS );
}

int
load ( const char * filename, std::string & mem )
{
	std::size_t   line_nb;
	std::string   line;
	std::ifstream file;
	
	if ( filename == 0x0 )
		return ( EXIT_FAILURE );
	file.open( filename );
	line_nb = 0;
	while ( file.good() == true && file.eof() == false )
	{
		std::getline( file, line );
		if ( normalize( line ).empty() == false )
			mem.append( line );
		++line_nb;
	}
	return ( EXIT_SUCCESS );
}

int
main ( int argc, const char ** argv )
{
	std::string		buffer;
	t_parse			parse_obj;

	std::memset( &parse_obj, 0, sizeof( parse_obj ) );
	if ( argc != 2 || argv[1] == 0x0 )
		return ( EXIT_FAILURE );
	if ( load( argv[1], buffer ) != EXIT_SUCCESS )
		return ( EXIT_FAILURE );
	if ( parse( buffer, &parse_obj ) != EXIT_SUCCESS )
		return ( EXIT_FAILURE );
	print_conf_item( &parse_obj.tree, 1 );
	return ( EXIT_SUCCESS );
}
