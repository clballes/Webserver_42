/*                                                                            */
/* main.cpp                                                                   */
/* mpuig-ma <mpuig-ma@student.42barcelona.com>                                */
/* Mon Mar 25 11:48:33 2024                                                   */

#include <iostream>
#include "define.hpp"
#include "debug.hpp"
#include "IEvent.hpp"
#include "Router.hpp"

int
IEvent::kq = 0;

int
main ( int argc, char * const * argv )
{
	Router router;
	
	if ( argc > 2 )
		return ( EXIT_FAILURE );
	router.load( argv[1] == 0x0 ? DEFAULT_CONF : argv[1] );
	return ( router.good() == false ? EXIT_FAILURE : router.listen() );
}
