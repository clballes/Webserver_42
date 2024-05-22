/*                                                                            */
/* main.cpp                                                                   */
/* mpuig-ma <mpuig-ma@student.42barcelona.com>                                */
/* Mon Mar 25 11:48:33 2024                                                   */

#include <cstdlib>		/* EXIT_SUCESS, EXIT_FAILURE */
#include "Router.hpp"	/* class Router */
#include "define.hpp"	/* DEFAULT_CONF */

void
set_verbose_level ( char * const * envp )
{
	const char		env_var[] = "DEBUG_LEVEL=";
	const size_t	len = std::strlen( env_var );
	size_t			iterator;
	char *			pos;

	#ifdef DEBUG_LEVEL
		_webserv_verbose_level = DEBUG_LEVEL;
	#endif
	iterator = 0;
	while ( envp != NULL && envp[iterator] != NULL )
	{
		pos = std::strchr( envp[iterator], '=' );
		if ( pos != NULL
				&& std::strncmp( envp[iterator], env_var, len ) == 0 )
		{
			if ( *( envp[iterator] + len ) == '\0' )
				_webserv_verbose_level = _MODE_NOTICE;
			else
				_webserv_verbose_level = std::atoi( envp[iterator] + len );
		}
		++iterator;
	}
	return ;
}

int
main ( int argc, char * const * argv, char * const * envp )
{
	Router router;

	set_verbose_level( envp );
	INFO( "verbose_level=" << _webserv_verbose_level );
	if ( argc > 2 )
	{
		std::cerr << "usage: " << EXEC_NAME << " [configuration file]\n";
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
