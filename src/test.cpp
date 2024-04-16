#include <sys/socket.h>		/* socket,
							   accept,
							   listen,
							   send,
							   recv,
							   bind,
							   connect,
							   getaddrinfo,
							   freeaddinfo,
							   setsockopt,
							   gettsockname */
#include <sys/types.h>		/* common for
							   gai_strerror,
							   kqueue,
							   connect,
							   getaddrinfo,
							   freeaddrinfo,
							   read */
#include <netdb.h>			/* common for
							   gai_strerror,
							   getaddrinfo,
							   freeaddrinfo,
							   getprotobynumber */
#include "Router.hpp"
#include <iostream>

int
IEvent::kq = 0;

int
main ( void )
{
	Router router;
	std::string host, port;
	struct sockaddr_in * address;
	struct addrinfo hints, * result;

	host.assign( "0.0.0.0" );
	port.assign( "8000" );
	std::memset( &hints, 0, sizeof( hints ) );
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;
	getaddrinfo( host.c_str(), port.c_str(), &hints, &result );
	address = (struct sockaddr_in * ) result->ai_addr;
	router.setConnection( *address );

	host.assign( "127.0.0.1" );
	port.assign( "9000" );
	std::memset( &hints, 0, sizeof( hints ) );
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;
	getaddrinfo( host.c_str(), port.c_str(), &hints, &result );
	address = (struct sockaddr_in * ) result->ai_addr;
	router.setConnection( *address );

	router.listen();

	return ( 0 );
}
