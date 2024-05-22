//                                                                             /
// client.c                                                                    /
// mpuig-ma <mpuig-ma@student.42barcelona.com>                                 /
// Tue May  7 11:10:07 2024                                                    /

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>

void
set_addrinfo_hints ( struct addrinfo *hints )
{
	memset( hints, 0, sizeof( *hints ) );
	hints->ai_family = AF_INET;
	hints->ai_socktype = SOCK_STREAM;
	hints->ai_protocol = IPPROTO_TCP;
	return ;
}

int
main ( int argc, char ** argv )
{
	struct addrinfo		hints, * res;
	int					ecode, socket_fd;
	char				* buffer_send;

	if ( argc != 3 )
	{
		fprintf( stderr, "usage: %s %s\n", argv[0], "host port" );
		return ( EXIT_FAILURE );
	}
	socket_fd = socket( AF_INET, SOCK_STREAM, IPPROTO_TCP );
	if ( -1 == socket_fd )
	{
		fprintf( stderr, "%s\n", strerror( errno ) );
		return ( EXIT_FAILURE );
	}
	printf( "socket=%d\n", socket_fd );
	res = NULL;
	set_addrinfo_hints( &hints );
	ecode = getaddrinfo( argv[1], argv[2], &hints, &res );
	if ( res == NULL )
	{
		fprintf( stderr, "%s:%s: %s\n", argv[1], argv[2],
				gai_strerror( ecode ) );
		return ( ecode );
	}
	if ( connect( socket_fd, (*res).ai_addr, (*res).ai_addrlen ) == -1 )
	{
		fprintf( stderr, "%s\n", strerror( errno ) );
		return ( EXIT_FAILURE );
	}
	printf( "Connected to %s:%s\n", argv[1], argv[2] );
	buffer_send = strdup(
			"GET / HTTP/1.1\r\n" 
			"Host: localhost:8000\r\n"
			"User-Agent: none\r\n" 
			"\r\n" );
	if ( send( socket_fd, buffer_send, strlen( buffer_send ), 0 ) == -1 )
	{
		fprintf( stderr, "%s\n", strerror( errno ) );
		return ( EXIT_FAILURE );
	}
	free( buffer_send );
	int n = getsockopt( socket_fd, SOL_SOCKET, SO_NREAD, 0, 0 );
	n = 200;
	char * buffer_recv = calloc( n + 1, sizeof( char ) );
	int rcode = recv( socket_fd, buffer_recv, n, 0 );
	printf( "rcode=%d\n", rcode );
	printf( "%s", buffer_recv );
	printf( "OK\n" );
	shutdown( socket_fd, 2 );
	return ( EXIT_SUCCESS );
}
