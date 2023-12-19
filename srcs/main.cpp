#include "./inc/Server.hpp"
// #include "./inc/ServerOptions.hpp"
#include <iostream>
#include <list>
#include <vector>

std::string trim_sp(const std::string& input);
std::string trim(const std::string& input);
// void        printArray();
void        addElements(std::string &line,  std::list<std::string>&listConfigig);
int         numServers(std::list<std::string>listConfig);
std::vector<std::list<std::string> >        splitServer(int servers, std::list<std::string>listConfig);
int         ParsingServers(std::string filename, std::list<std::string>&listConfig);
int         check_brackets(std::list<std::string>&listConfig);


int main(int argc, char *argv[])
{
    if (argc != 2){
        std::cerr << "ERROR!" << std::endl;
        return (1);
    }
    else{
        std::list<std::string>listConfig;
        int res = ParsingServers(argv[1], listConfig);
        if(res){
            std::cerr << "Cannot execute; parsing incorrect syntax" << std::endl;
            return (1);
        }
        try{
            std::vector<std::list<std::string> >arrayOfLists = splitServer(numServers(listConfig), listConfig);
            std::vector<Server> serverVector;
            serverVector.reserve(arrayOfLists.size()); 
            for (size_t i = 0; i < arrayOfLists.size(); ++i) {
                serverVector.push_back(Server(arrayOfLists[i]));
            }
        }
        catch (const std::exception & e)
        {
            std::cout << e.what() << std::endl;
        }
    }
    return (0);
}