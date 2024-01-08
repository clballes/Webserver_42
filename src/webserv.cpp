/*                                                                            */
/* webserv.cpp                                                                */
/* clballes <clballes@student.42barcelona.com>                                */
/* Mon Jan  8 12:17:22 2024                                                   */

#include "webserv.hpp"
#include "ServerConf.hpp" // per separat o incloure a webserv.hpp ??
#include <iostream>

int
main (int argc, const char **argv)
{
	std::string		conf_file;

	conf_file = DEFAULT_CONF;
    if (argc > 2)
    {
		std::cout << argv[0] << " [configuration file]";
		std::cout << std::endl;
		return (EXIT_SUCCESS);
    }
	if (argv[1] != NULL)
		conf_file.assign(argv[1]);

	// Podriem fer:
	// ServerConf	server_conf(conf_file);
	ServerConf	server_conf;

	return (EXIT_SUCCESS);
}
