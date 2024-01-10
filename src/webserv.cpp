/*                                                                            */
/* webserv.cpp                                                                */
/* clballes <clballes@student.42barcelona.com>                                */
/* Mon Jan  8 12:17:22 2024                                                   */

#include "Server.hpp"
#include "ParsingServers.hpp"
#include <iostream>
#include <list>

int
main (int argc, const char **argv)
{
	std::string		conf_file;

	#ifdef SILENCE_LOGS
	std::ofstream	log_file("logs.txt");
	std::clog.rdbuf(log_file.rdbuf());
	#endif

	conf_file = DEFAULT_CONF;
    if (argc > 2)
    {
		std::cout << argv[0] << " [configuration file]";
		std::cout << std::endl;
		return (EXIT_SUCCESS);
    }
	if (argv[1] != NULL)
		conf_file.assign(argv[1]);

	Server	server_instance;

	return (EXIT_SUCCESS);
}
