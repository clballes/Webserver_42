// #include "./inc/Server.hpp"
#include "./inc/ServerOptions.hpp"

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        std::cerr << "ERROR!" << std::endl;
        return 1;
    }
    else
    {
        ServerOptions serverConf(argv[1]);
        try
        {
            // config.ConfigServer(argv[1]);
            // config.configLocation();
            // std::cout << "servername: "<< config.getServer().server_name << std::endl;
            // std::cout << "location: "<< config.getServer().mapLocations->path << std::endl;
        }
        catch(const std::exception & e)
        {
            std::cout << e.what() << std::endl;
        }
    }
    return 0;
}