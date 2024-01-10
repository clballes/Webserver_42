/*                                                                            */
/* webserv.cpp                                                                */
/* clballes <clballes@student.42barcelona.com>                                */
/* Mon Jan  8 12:17:22 2024                                                   */

#include "webserv.hpp"
#include "ServerConf.hpp" // per separat o incloure a webserv.hpp ??
#include <iostream>
#include <list>

//functions for parsing
std::string trim_sp(const std::string& input);
std::string trim(const std::string& input);
// void        printArray();
void        addElements(std::string &line,  std::list<std::string>&listConfigig);
int         numServers(std::list<std::string>listConfig);
std::vector<std::list<std::string> >        splitServer(int servers, std::list<std::string>listConfig);
int         parsingServers(std::string filename, std::list<std::string>&listConfig);
int         check_brackets(std::list<std::string>&listConfig);


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
	// ServerConf	server_conf(conf_file);
	// ServerConf	server_conf;

	return (EXIT_SUCCESS);
}
