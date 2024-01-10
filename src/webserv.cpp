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

	conf_file = DEFAULT_CONF;
    if (argc > 2)
    {
		std::cout << argv[0] << " [configuration file]";
		std::cout << std::endl;
		return (EXIT_SUCCESS);
    }
	if (argv[1] != NULL)
		conf_file.assign(argv[1]);

	// parsing servers posa tot en una llista
	std::list<std::string>listConfig;
	if(parsingServers(argv[1], listConfig) == EXIT_FAILURE){
		return EXIT_FAILURE;
	}
		// AIXO PER FER ARRAY DE SERVERS
		// std::vector<std::list<std::string> >arrayOfLists = splitServer(numServers(listConfig), listConfig);
		// std::cout << arrayOfLists.size() << std::endl;
		// std::vector<Server> serverVector(2); //array or dque han de ser punters
		// // serverVector.reserve(2); 
		// 	// std::cout << "a" << std::endl;
		// for (size_t i = 0; i < 2; ++i) { //construy dos veces el copy y el construcotrrrr, entonces se me destruye dos veces tbien
		// 	serverVector[i] = Server(); //Server(arrayOfLists[i]);
		// }
	return (EXIT_SUCCESS);
}
