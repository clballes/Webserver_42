/*                                                                            */
/* main.cpp                                                                   */
/* mpuig-ma <mpuig-ma@student.42barcelona.com>                                */
/* Mon Mar 25 11:48:33 2024                                                   */

#include <iostream>
#include "define.hpp"
#include "debug.hpp"
#include "Controller.hpp"
#include "IEvent.hpp"

int
IEvent::kq = 0;

int
main ( int argc, char * const * argv )
{
	Controller controller;
	
	if ( argc > 2 )
		return ( EXIT_FAILURE );

	controller.load( argv[1] == 0x0 ? DEFAULT_CONF : argv[1] );
	if ( controller.good() == false )
		return ( EXIT_FAILURE );
	return ( controller.start() );
}
