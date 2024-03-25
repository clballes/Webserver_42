#include "ServerHandler.hpp"

ServerHandler::ServerHandler(){
    std::cout << "call constructr server handler"<< std::endl;
}

ServerHandler::~ServerHandler(){
    std::cout << "call destructor server handler"<< std::endl;
}

const std::vector<ServerConf>& ServerHandler::getServers() const
{
    return _servers;
}
