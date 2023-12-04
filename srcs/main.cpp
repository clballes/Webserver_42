#include "./inc/Config.hpp"

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        std::cerr << "ERROR!" << std::endl;
    }
    else
    {
        Config config;
        // Config config = { 0 };
        try 
        {
            config.ConfigList(argv[1]);
        }
        catch(const std::exception & e)
        {
            std::cout << e.what() << std::endl;
        }
    }
    return 0;
}