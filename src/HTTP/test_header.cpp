#include <iostream>
#include <unistd.h>
#include <fstream>

#include <sstream>
#include <string>

void
parse_header ( std::string & line )
{
	std::cout << "> " << line << std::endl;

	if ( line.at( 0 ) != 'G' || line.at( 0 ) != 'P' || line.at( 0 ) != 'D' )
	
	return ;
}

void
parse_headers ( char * buffer )
{
	std::istringstream iss( buffer );
	std::string line;

	while ( iss.good() == true && iss.eof() == false )
	{
		std::getline( iss, line, '\n' );
		parse_header( line );
	}

	return ;
}

int
main ( int argc, char ** argv )
{
	char *  _buffer_recv;
	int64_t _data_recv;

	if ( argc != 2 )
		return ( EXIT_FAILURE );

	_buffer_recv = argv[1];
	_data_recv = strlen( _buffer_recv );

	//write( STDOUT_FILENO, _buffer_recv, _data_recv );
	parse_headers ( _buffer_recv );

	return ( EXIT_SUCCESS );
}
