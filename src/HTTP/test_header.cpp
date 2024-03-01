#include <iostream>
#include <unistd.h>
#include <fstream>

#include <sstream>
#include <string>

class Buffer
{
	Buffer ( int64_t );
	~Buffer ( void );
};

int
main ( int argc, char ** argv )
{
	char *  _buffer_recv;
	int64_t _data_recv;

	if ( argc != 2 )
		return ( EXIT_FAILURE );

	_buffer_recv = argv[1];
	_data_recv = strlen( _buffer_recv );

	s = _buffer_recv;

	return ( EXIT_SUCCESS );
}
