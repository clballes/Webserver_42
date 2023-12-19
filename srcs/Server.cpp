#include "./inc/Server.hpp"

// ---------------- INIZIALIZATION FUNCTIONS -----------------------

Server:: Server()
{
    std::cout << "Constructor " << std::endl;

}


Server::Server(const std::list<std::string>& list)
{
    std::cout << "Constructor default called Server" << std::endl;
    (void)list;
    // populateServer(list);
}

Server::Server(const Server &options)
{
    (void)options;
    std::cout << "Copy constructor called " <<  std::endl;
}


// Server & Server::operator=(Server & src)
// {
//     this->map = src.map;
//     std::cout << "Operator called" << std::endl;
//     return *this;
// }

Server::~Server()
{
    std::cout << "Destructor called Server" << std::endl;
}

// ------------------------- MEMBER FUNCTIONS -----------------------------------

void Server::populateServer(std::list<std::string>listServer)
{
    std::list<std::string>::iterator itB = listServer.begin();
    std::list<std::string>::iterator itLoc = listServer.end();
    for (; itB != listServer.end(); ++itB) {
        if (itB->find("location") != std::string::npos) {
            itLoc = itB;
            break;
        }
    }
    std::list<std::string>::iterator it = listServer.begin();
    for(; it != itLoc; ++it){
        if(it->find("server_name") != std::string::npos){
            this->server_name = it->substr(12, it->length());
        }
        else if(it->find("listen") != std::string::npos){
            this->listen = it->substr(7, it->length());
        }
        else if(it->find("root") != std::string::npos){
            this->root = it->substr(5, it->length());
        }
        else if(it->find("index") != std::string::npos){
            this->index = it->substr(6, it->length());
        }
        else if(it->find("allow_methods") != std::string::npos)
        {
            this->allow_methods = it->substr(14, it->length());
        }
    }
}
