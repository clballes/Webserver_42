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
		// decode_arguments( argc, argv );
		conf_filename = (argv[optind] == 0x0 ? DEFAULT_CONF : argv[optind] );
		if (cluster.config( conf_filename ) == EXIT_FAILURE)
		{
			std::cerr << "Error in the main of webserver config" << std::endl;
			return (EXIT_FAILURE);
		}
		if (cluster.setup() == -1)
		{
			std::cerr << "Error in the main of webserver confi 2" << std::endl;
			return (EXIT_FAILURE);
		}
		if (cluster.webserver() == -1)
		{
			std::cerr << "Error in the main of webserver config 3" << std::endl;
			return (EXIT_FAILURE);
		}
		exit(1);

			// cluster.clean();			
	}
	else
	{
		std::cerr << "Invalid number of arguments." << std::endl;
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}
