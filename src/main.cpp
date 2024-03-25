/*                                                                            */
/* main.cpp                                                                   */
/* mpuig-ma <mpuig-ma@student.42barcelona.com>                                */
/* Thu Jan 25 14:26:11 2024                                                   */

#include "webserv.hpp"
#include "Server.hpp"
#include "init.hpp"
#include "parse.hpp"
#include "Cluster.hpp"
#include <fstream>

int
main ( int argc, char * const * argv )
{
	std::string   conf_filename;

	if (argc == 2)
	{
		Cluster		cluster;

		// Check / set argument options
		decode_arguments( argc, argv );
		conf_filename = (argv[optind] == 0x0 ? DEFAULT_CONF : argv[optind] );
		if (cluster.config( conf_filename ) == EXIT_FAILURE)
		{
			std::cerr << "Error in the main of webserver config" << std::endl;
			return (EXIT_FAILURE);
		}
		exit(1);

			// if (cluster.setup() == -1)
			// 	return (1);
			// cluster.run();
			// cluster.clean();			
	}
	else
	{
		std::cerr << "Invalid number of arguments." << std::endl;
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);




	// // Initialize n Server instances based on the configuration file.
	// // new `Server's in Server::servers will have to be FREEd.

	// for ( ServerConf::iterator it = ServerConf::instances.begin();
	// 		it != ServerConf::instances.end(); ++it )
	// {
	// 	Server::servers.push_back( new Server( *( *it ) ) );
	// 	if ( ! Server::servers.back()->good )
	// 	{
	// 		Server::clear();
	// 		return ( EXIT_FAILURE );
	// 	}

	// }

	// ServerConf::clear();
	// Server::servers.shrink_to_fit();

	// // Start program's purpose.

	// if ( ::webserv() != EXIT_SUCCESS )
	// {
	// 	std::cerr << "Quitting ";
	// 	std::cerr << PROGRAM_NAME;
	// 	std::cerr << std::endl;
	// 	return ( EXIT_FAILURE );
	// }

	// return ( EXIT_SUCCESS );
}
