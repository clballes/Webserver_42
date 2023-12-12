// #include "./inc/Server.hpp"
#include "./inc/ServerOptions.hpp"
#include "./inc/ParsingServers.hpp"

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        std::cerr << "ERROR!" << std::endl;
        return 1;
    }
    else
    {
        ParsingServers parsing(argv[1]);
        std::vector<std::list<std::string>*>arrayOfLists = parsing.getArrList();
        try
        {
            for (unsigned long int i = 0; i < parsing.getNumServer(); ++i)
            {
                ServerOptions serverConf(arrayOfLists[i]);
                serverConf.parseConfigFile();
                // std::cout << "-------------Sublist------------" << i + 1 << std::endl;
            }
        }
        catch(const std::exception & e)
        {
            std::cout << e.what() << std::endl;
        }
    }
    return 0;
}