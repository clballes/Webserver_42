#include "./inc/Server.hpp"

// ---------------- INIZIALIZATION FUNCTIONS -----------------------

Server:: Server(std::list<std::string>list2)
{
    std::cout << "Constructor called " << std::endl;
    // std::list<std::string>::iterator it = list2.begin();
    // for(; it != list2.end(); ++it)
    // {
    //     std::cout << *it << std::endl;
    // }
    populateServer(list2);
}

Server::Server(const ServerOptions &options)
{
    (void)options;
    std::cout << "Constructor called " <<  std::endl;
    //constructr checkeo el Server, pasarli el fd
}

Server::Server(Server & src)
{
    std::cout << "Copy called" << std::endl;
    *this = src;
}

// Server & Server::operator=(Server & src)
// {
//     this->map = src.map;
//     std::cout << "Operator called" << std::endl;
//     return *this;
// }

Server::~Server()
{
    std::cout << "Destructor called" << std::endl;
}

// ------------------------- MEMBER FUNCTIONS -----------------------------------

void Server::populateServer(std::list<std::string>listServer)
{
    std::list<std::string>::iterator itB = listServer.begin();
    std::list<std::string>::iterator itLoc = listServer.end();

    // Find the iterator pointing to the first occurrence of "location"
    for (; itB != listServer.end(); ++itB) {
        if (itB->find("location") != std::string::npos) {
            itLoc = itB;
            break;
        }
    }
    std::list<std::string>::iterator it = listServer.begin();
    for(; it != itLoc; ++it)
    {
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
    // std::cout << this->server_name << std::endl;
    // std::cout << this->index << std::endl;
    // std::cout << this->allow_methods << std::endl;
    // std::cout << this->root << std::endl;
}
