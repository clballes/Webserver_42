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

#define no	false
#define yes	true

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

std::size_t
next_context ( std::string & buffer, std::size_t pos, std::string & context )
{
	std::size_t len, it, lvl;

	lvl = 0;
	len = 0;
	it = pos;
	while ( it != buffer.length() )
	{
		if ( buffer[it] == '{' )
			++lvl;
		if ( buffer[it] == '}' )
			--lvl;
		if ( buffer[it] == '}' && lvl == 0 )
		{
			++len;
			break ;
		}
		++len;
		++it;
	}
	context.assign ( buffer.substr( pos, len == 0 ? 1 : len ) );
	return ( len == 0 ? 1 : len );
}
int
load ( std::string & filename, std::string & mem )
{
	std::size_t   line_nb;
	std::string   line;
	std::ifstream file;
	
	file.open( filename );
	line_nb = 0;
	while ( file.good() == true && file.eof() == false )
	{
		std::getline( file, line );
		if ( normalize( line ).empty() == false )
			mem.append( line ).append( " " );
		++line_nb;
	}
	trim_f( mem, &std::isspace );
	return ( EXIT_SUCCESS );
}

std::size_t
how_many_options_are_there ( const t_conf_opts * opts )
{
	std::size_t n;

	n = 0;
	while ( opts != NULL && opts[n].identifier != 0x0 )
		++n;
	return ( n );
}

int
parse_context ( std::string & context, const t_conf_opts * opts )
{
	DEBUG( context.c_str() );
	(void) opts;
	return ( EXIT_SUCCESS );
}

int
parse_token ( std::string & token, const t_conf_opts * opts )
{
	const std::size_t 	opts_len = how_many_options_are_there( opts );
	std::size_t			i;
	std::string			word;

	// TODO: fix.
	// temporal measure: if token has no content, do nothing
	if ( token.empty() == true )
		return ( EXIT_SUCCESS );

	DEBUG( token.c_str() );
	i = 0;
	if ( token.compare( "{" ) == 0 )
	{
		DEBUG( "++context" );
		return ( EXIT_SUCCESS );
	}
	if ( token.compare( "}" ) == 0 )
	{
		DEBUG( "--context" );
		return ( EXIT_SUCCESS );
	}

	word = token.substr( 0, token.find_first_of( " " ) );
	while ( i < opts_len && word.compare( opts[i].identifier ) != 0 )
		++i;
	
	if ( i == opts_len )
	{
		ERROR( "unknown directive \"" << word.c_str() << "\"" );
		return ( EXIT_FAILURE );
	}
	if ( opts[i].type == DIRECTIVE && token.back() != ';' )
	{
		ERROR( "directive \"" << word << " is not terminated by \";\"" );
		return ( EXIT_FAILURE );
	}

	return ( EXIT_SUCCESS );
}

int
parse ( std::string filename )
{
	std::size_t			pos = 0;
	std::string			buffer, context, token;
	t_conf_item			tree;
	const t_conf_opts	opts[] =
	{
		{ CONTEXT, "events", no, "main", 0x0 },
		{ CONTEXT, "http", no, "main", 0x0 },
		{ CONTEXT, "server", yes, "http", 0x0 },
		{ CONTEXT, "location", yes, "server,location", 0x0 },
		{ DIRECTIVE, "server_name", no, "server", 0x0 },
		{ DIRECTIVE, "listen", no, "server", 0x0 },
		{ DIRECTIVE, "allow_methods", no, "server,location", 0x0 },
		{ DIRECTIVE, "root", no, "server", 0x0 },
		{ DIRECTIVE, "index", no, "server", 0x0 },
		{ DIRECTIVE, "autoindex", no, "server", 0x0 },
		{ DIRECTIVE, "cgi_pass", no, "server,location", 0x0 },
		{ DIRECTIVE, "cgi_param", no, "server,location", 0x0 },
		{ 0, 0x0, 0, 0x0, 0x0 }
	};

	std::memset( &tree, 0, sizeof( tree ) );
	load( filename, buffer );
	while ( pos != buffer.length() )
	{
		pos += next_token( buffer, pos, token );
		trim_f( token, &std::isspace );
		if ( parse_token( token, opts ) == EXIT_FAILURE )
			return ( EXIT_FAILURE );
		//pos += next_context( buffer, pos, context );
		//parse_context( context, (t_conf_opts *) &opts );
	}
	print_conf_item( &tree, 1 );
	return ( EXIT_SUCCESS );
}

#define DEFAULT_CONF "conf/default.conf"

int
main ( int argc, const char ** argv )
{
	if ( argc != 2 )
		return ( EXIT_FAILURE );
	if ( parse( argv[1] == 0x0 ? DEFAULT_CONF : argv[1] ) != EXIT_SUCCESS )
		return ( EXIT_FAILURE );
	return ( EXIT_SUCCESS );
}
