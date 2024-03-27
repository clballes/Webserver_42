/*                                                                            */
/* test.cpp                                                                   */
/* mpuig-ma <mpuig-ma@student.42barcelona.com>                                */
/* Tue Mar 26 12:29:29 2024                                                   */

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

size_t
count ( const char * str, char c )
{
	size_t nb = 0;

	while ( str != 0 && *str != '\0' )
	{
		if ( *str == c )
			++nb;
		++str;
	}

	return ( nb );
}

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

void
evaluate_token( std::string & token )
{
	std::cout << token << '\n';
	return ;
}

int
evaluate ( std::string & line, t_conf_item * tree, const t_conf_opts * opts )
{
	std::string::size_type curly_braces = 0;
	std::string token;
	
	DEBUG ( "\"" << line << "\"" );	
	// TODO: if '{' ++, if '}' --

	std::string::size_type start = 0;
	std::string::size_type pos = 0;
	while ( line[pos] != '\0' )
	{
		
		evaluate_token( token );
	}

	(void) tree;
	(void) opts;
	(void) curly_braces;
	return ( EXIT_SUCCESS );
}

t_conf_item *
parse ( std::ifstream & file, const t_conf_opts * opts )
{
	std::size_t   n_line = 0;
	std::string   line;
	t_conf_item * tree = new t_conf_item;

	std::memset( tree, 0, sizeof( *tree ) );
	while ( file.eof() == false && file.good() == true )
	{
		std::getline( file, line );
		trim_f( line, &std::isspace );
		if ( line.empty() == false && line.front() != '#'
				&& evaluate( line, tree, opts ) != EXIT_SUCCESS )
		{
			ERROR( "line: " << n_line );
			return ( NULL );
		}
		++n_line;
	}
	return ( tree );
}

int
main ( int argc, char ** argv )
{
	std::ifstream file;
	t_conf_item * tree = 0x0;
	
	const t_conf_opts opts[] =
	{
		{ CONTEXT, "http", false, "main", 0x0 },
		{ CONTEXT, "server", true, "http", 0x0 },
		{ CONTEXT, "location", true, "server,location", 0x0 },
		{ DIRECTIVE, "server_name", false, "server", 0x0 },
		{ 0, 0x0, 0, 0x0, 0x0 }
	};
	
	if ( argc != 2 )
		return ( EXIT_FAILURE + 1 );

	file.open ( argv[1] );
	if ( file.good() == false )
		return ( EXIT_FAILURE + 2 );

	tree = parse( file, opts );
	if ( tree == NULL )
		return ( EXIT_FAILURE + 3 );

	print_conf_item( tree, 1 );
	clear_parse_tree ( tree );

	return ( EXIT_SUCCESS );
}
