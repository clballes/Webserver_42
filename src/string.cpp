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
			break ;
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

std::size_t
how_many_characters_of ( const std::string & str, char c )
{
	std::string::const_iterator it;
	std::size_t n = 0;

	it = str.begin();
	while ( it != str.end() )
	{
		if ( c == *it )
			++n;
		++it;
	}
	return ( n );
}

std::size_t
how_many_words ( const std::string & str )
{
	std::string::const_iterator it;
	std::size_t n = 1;

	it = str.begin();
	while ( it != str.end() )
	{
		if ( std::isspace( *it )
				&& it != str.begin()
				&& std::isspace( *( it - 1 ) ) == 0)
			++n;
		++it;
	}
	return ( ( str.empty() == true ) ? 0 : n );
}

std::string
get_word ( const std::string & str, std::string delimiter )
{
	std::string word;

	if ( str.find_first_of( delimiter ) != std::string::npos )
		word.assign( str.substr( 0, str.find_first_of( delimiter ) ) );
	return ( word );
}

int
unchunk ( const std::string & src, std::string & dst )
{
	std::string::size_type	pos, next_crlf_pos, chunk_length;
	std::string				temp, chunk_length_hex;

	pos = 0;
	while ( pos < src.length() )
	{
		next_crlf_pos = src.find( "\r\n", pos );
		if ( next_crlf_pos == std::string::npos )
			return ( EXIT_FAILURE );
		chunk_length_hex = src.substr( pos, next_crlf_pos - pos );
		chunk_length = std::strtol( chunk_length_hex.c_str(), NULL, 16 );
		pos = next_crlf_pos + 2;
		temp.append( src.substr( pos, chunk_length ) );
		pos += chunk_length + 2;
		if ( chunk_length == 0 )
			break ;
	}
	dst.append( temp );
	return ( EXIT_SUCCESS );
}

bool
compare_file_extension( const std::string & a, const std::string & b )
{
	std::string::size_type position;
	std::string ext_a, ext_b;

	if ( a.empty() || b.empty() )
		return ( false );
	if ( how_many_characters_of( a, '.' ) == 0
			|| how_many_characters_of( b, '.' ) == 0 )
		return ( false );
	position = a.find_last_of( "." );
	ext_a = a.substr( position, std::string::npos );
	position = b.find_last_of( "." );
	ext_b = b.substr( position, std::string::npos );
	if ( ext_a == ext_b )
		return ( true );
	return ( false );
}

template< typename T >
std::string my_to_string ( const T & value )
{
    std::stringstream ss;

    ss << value;
    return ( ss.str() );
}

template std::string my_to_string< int > ( const int & );
template std::string my_to_string< unsigned long > ( const unsigned long & );
template std::string my_to_string< unsigned short > ( const unsigned short & );
