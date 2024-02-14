/*                                                                            */
/* main.cpp                                                                   */
/* mpuig-ma <mpuig-ma@student.42barcelona.com>                                */
/* Thu Jan 25 14:26:11 2024                                                   */

#include "webserv.hpp"
#include "init.hpp"

#include <fstream>
#include <set>

#define LOG(s) { std::clog << s << std::endl; }

int
main ( int argc, char * const * argv )
{
	// Check / set argument options

	decode_arguments( argc, argv );

	// Open ( or try to ) a configuration file.
	// If none is set ( == NULL ), falls back to using DEFAULT_CONF ( macro )

	std::ifstream config_file;
	config_file.open( argv[optind] == 0x0 ? DEFAULT_CONF : argv[optind] );

	if ( config_file.good() != true )
	{
		std::cerr << ( argv[optind] == 0x0 ? DEFAULT_CONF : argv[optind] );
		std::cerr << ": " << ::strerror( errno ) << std::endl;
		return ( EXIT_FAILURE );
	}

	// Parse configuration file.
	// ...
	//
	
	// Once done, close file;

	config_file.close();

	// Initialize n Server instances based on the configuration file.
	// ...
	
		// For the purpose of testing;
		// initialize some servers manually.

		std::set< const Server * > servers;

		Server a( 8080 );
		Server b( 80 );
		Server c( 424242 );

		servers.insert( &a );
		servers.insert( &b );
		servers.insert( &c );

	// Start listening / accepting connections
	// ... kqueue
	
	::webserv( servers );

	return ( EXIT_SUCCESS );
}
