/*                                                                            */
/* main.cpp                                                                   */
/* mpuig-ma <mpuig-ma@student.42barcelona.com>                                */
/* Mon Mar 25 11:48:33 2024                                                   */

#include <iostream>
#include "define.hpp"
#include "debug.hpp"
#include "IEvent.hpp"
#include "Controller.hpp"
#include "Router.hpp"

int
IEvent::kq = 0;

/*
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
*/

int
main ( int argc, char * const * argv )
{
	Router router;
	
	if ( argc > 2 )
		return ( EXIT_FAILURE );

	(void) argv;
	//if ( parse( argv[1] == 0x0 ? DEFAULT_CONF : argv[1] ) == EXIT_FAILURE )
	//	return ( EXIT_FAILURE );

	//router.load( configuration_object_from_parse );
	return ( router.listen() );
}

int
parse ( )
{
	return ( EXIT_SUCCESS );
}
