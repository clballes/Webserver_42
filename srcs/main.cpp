// #include "./inc/Server.hpp"
#include "./inc/ServerOptions.hpp"
// #include "./inc/ParsingServers.hpp"
#include <iostream>
#include <list>

std::string trim3(const std::string& input);
std::string trim2(const std::string& input);
void        printArray();
void        parsingBrackets(std::string &line,  std::list<std::string>&listConfig);
int        numServers(std::list<std::string>listConfig);
void        splitServer();
void        ParsingServers(std::string filename, std::list<std::string>&listConfig);


int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        std::cerr << "ERROR!" << std::endl;
        return 1;
    }
    else
    {
        std::list<std::string>listConf;
        ParsingServers(argv[1], listConf);
        // std::vector<std::list<std::string>*>arrayOfLists = parsing.getArrList();
        try
        {
            // for (unsigned long int i = 0; i < 2; ++i) //vectr size
            // {
            //     ServerOptions serverConf(arrayOfLists[i]);
            //     serverConf.parseConfigFile();
            // }
        }
        catch(const std::exception & e)
        {
            std::cout << e.what() << std::endl;
        }
    }
    return 0;
}