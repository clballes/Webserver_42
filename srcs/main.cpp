// #include "./inc/Server.hpp"
#include "./inc/ServerOptions.hpp"
#include <iostream>
#include <list>

std::string trim_sp(const std::string& input);
std::string trim(const std::string& input);
// void        printArray();
void        addElements(std::string &line,  std::list<std::string>&listConfig);
int        numServers(std::list<std::string>listConfig);
// void        splitServer();
int        ParsingServers(std::string filename, std::list<std::string>&listConfig);
int        check_brackets(std::list<std::string>&listConfig);


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
        int res = ParsingServers(argv[1], listConf);
        if(res)
        {
            std::cerr << "Cannot execute; parsing incorrect syntax" << std::endl;
        }
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