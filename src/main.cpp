/*                                                                            */
/* main.cpp                                                                   */
/* mpuig-ma <mpuig-ma@student.42barcelona.com>                                */
/* Mon Mar 25 11:48:33 2024                                                   */

#include "Controller.hpp"
#include <iostream>

#include <getopt.h>
#include "define.hpp"

#include "debug.hpp"

// TODO: envp -> DEBUG variables / mode

static int decode_arguments ( int, char * const * );

int
main ( int argc, char * const * argv )
{
	DEBUG( "" );
	
	Controller controller;
	
	decode_arguments ( argc, argv );
	controller.load( argv[optind] == 0x0 ? DEFAULT_CONF : argv[optind] );

	return ( controller.start() );
}

int
decode_arguments ( int argc, char * const * argv )
{
	DEBUG( "" );
	
	int					c = 0, cc = 0;
	int 				option_index = 0;
	const char *		shortopts = "vm:l:";
	
	const struct option longopts[] =
	{
		{ "version", no_argument, 0x0, 'v'},
		{ "log", required_argument, 0x0, 'l'},
		{ "mode", required_argument, 0x0, 'm'},
		{ 0, 0, 0, 0 }
	};
	
	while ( c != -1 )
	{
		c = getopt_long ( argc, argv, shortopts, longopts, &option_index );
		(void) cc;
		//cc = getoptions_long ( argc, argv, shortopts, longopts, &option_index );

		switch ( c )
		{
			case 'v':
				std::cout << WEBSERV_VER << std::endl;
				exit (EXIT_SUCCESS);
			case 'l':
				std::cout << "logging into " << optarg << std::endl;
				break ;
			case 'm':
				std::cout << "mode: " << optarg << std::endl;
				break ;
			case '?':
				exit (EXIT_FAILURE);
			default:
				break ;
		}
	}
	
	return (0);
}

/*
int
main ( int argc, char * const * argv )
{
	std::string   conf_filename;
	std::ifstream conf_file;

	// Check / set argument options

	decode_arguments( argc, argv );
	
	// If argv[optind] is set ( == NULL ),
	// falls back to using DEFAULT_CONF ( macro )
	
	conf_filename = (argv[optind] == 0x0 ? DEFAULT_CONF : argv[optind] );

	// is_regular_file() is in `parse.cpp', for the moment.
	// Check if `conf_filename' is a regular file.

	if ( is_regular_file( conf_filename ) == false )
	{
		std::cerr << PROGRAM_NAME;
		std::cerr << ": " << conf_filename;
		std::cerr << ": " << ::strerror( errno ) << std::endl;
		return ( EXIT_FAILURE );
	}
	
	// Open ( or try to ) `conf_file'.
	
	conf_file.open( conf_filename );

	// Check if file has been properly opened.

	if ( conf_file.good() != true )
	{
		std::cerr << ( argv[optind] == 0x0 ? DEFAULT_CONF : argv[optind] );
		std::cerr << ": " << ::strerror( errno ) << std::endl;
		return ( EXIT_FAILURE );
	}
	LOG( conf_filename << ": open OK" );

	// Add `conf_file' is contents.
	// Server_conf::add() also parses them. 
	
	if ( ServerConf::add( conf_file ) == EXIT_FAILURE )
	{
		conf_file.close();
		return ( EXIT_FAILURE );
	}
	
	// Once done, close file;

	conf_file.close();
	
	LOG( conf_filename << ": closed OK" );

	// Initialize n Server instances based on the configuration file.
	// new `Server's in Server::servers will have to be FREEd.

	for ( ServerConf::iterator it = ServerConf::instances.begin();
			it != ServerConf::instances.end(); ++it )
	{
		Server::servers.push_back( new Server( *( *it ) ) );
		if ( ! Server::servers.back()->good )
		{
			Server::clear();
			return ( EXIT_FAILURE );
		}

	}

	ServerConf::clear();
	Server::servers.shrink_to_fit();

	// Start program's purpose.

	if ( ::webserv() != EXIT_SUCCESS )
	{
		std::cerr << "Quitting ";
		std::cerr << PROGRAM_NAME;
		std::cerr << std::endl;
		return ( EXIT_FAILURE );
	}

	return ( EXIT_SUCCESS );
}
*/
