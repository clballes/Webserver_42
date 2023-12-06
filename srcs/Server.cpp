#include "./inc/Server.hpp"

// ---------------- INIZIALIZATION FUNCTIONS -----------------------

Server::Server()
{
    std::cout << "Constructor called " << std::endl;
    //constructr checkeo el Server, pasarli el fd
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

void Server::populateServer()
{
    this->server_name = this->mapConfigServer["server_name"];
    this->listen = this->mapConfigServer["listen"];
    this->root = this->mapConfigServer["root"];
    this->index = this->mapConfigServer["index"];
    this->allow_methods = this->mapConfigServer["allow_methods"];
    (void)this->server_address;
}
