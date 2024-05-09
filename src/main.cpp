/*                                                                            */
/* main.cpp                                                                   */
/* mpuig-ma <mpuig-ma@student.42barcelona.com>                                */
/* Mon Mar 25 11:48:33 2024                                                   */

#include <cstdlib>		/* EXIT_SUCESS, EXIT_FAILURE */
#include "Router.hpp"	/* class Router */
#include "define.hpp"	/* DEFAULT_CONF */

int
main ( int argc, char * const * argv )
{
	Router router;

	_webserv_verbose_level = _MODE_LOG;
	INFO( "verbose_level=" << _webserv_verbose_level );
	if ( argc > 2 )
	{
		LOG( "usage: " << EXEC_NAME << " [configuration file]" );
		return ( EXIT_FAILURE );
	}
	router.load( argv[1] == 0x0 ? DEFAULT_CONF : argv[1] );
	return ( router.good() == false ? EXIT_FAILURE : router.listen() );
}

/* 
 * Note for the future.
 * The Router class might be split into a Router class and a Controller class.
 * The Controller shoud handle the configuration of servers whilst
 * the Router shoudl route connections to each corresponding server
 * and is instantiated by the Controller.
 *
 * As of now the class Router does both functions 
 * which -we must admit- is kind of weird.
 *
 */
