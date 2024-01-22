/*                                                                            */
/* webserv.cpp                                                                */
/* clballes <clballes@student.42barcelona.com>                                */
/* Mon Jan  8 12:17:22 2024                                                   */

#include "Server.hpp"
#include "ParsingServers.hpp"
#include <iostream>
#include <fstream>
#include <list>

int
main (int argc, const char **argv)
{
	std::string		conf_file;

	#ifdef SILENCE_LOGS
	std::ofstream log_file;
	log_file.open("logs.txt", std::ostream::app);
	std::clog.rdbuf(log_file.rdbuf());
	#endif

	std::clog << "webserv" << std::endl;

	conf_file.assign(DEFAULT_CONF);
    if (argc > 2)
    {
		std::cout << argv[0] << " [configuration file]";
		std::cout << std::endl;
		return (EXIT_SUCCESS);
    }

	if (argv[1] != NULL)
		conf_file.assign(argv[1]);
	
	if (conf_file.compare("-v") == 0
		|| conf_file.compare("--version") == 0)
	{
		std::cout << WEBSERV_VER << std::endl;
		return (EXIT_SUCCESS);
	}

	Server* server_instance = new Server;
	server_instance->setName("puyma.xyz");
	server_instance->setPort(8000);
	server_instance->setAddr(INADDR_ANY);
	server_instance->setAddr(0x1C1C1C1C);

	std::cout << *server_instance << std::endl;

	if (server_instance->ok() == WEBSERV_OK)
		server_instance->listen();

	delete server_instance;
	return (EXIT_SUCCESS);
}
