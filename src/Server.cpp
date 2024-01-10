/*                                                                            */
/* Server.cpp                                                                 */
/* clballes <clballes@student.42barcelona.com>                                */
/* Mon Jan  8 12:39:29 2024                                                   */

#include "Server.hpp"

// ---------------- INIZIALIZATION FUNCTIONS -----------------------
Server::Server (void)
{
    std::cout << "Constructor called " << std::endl;
}

Server::Server (Server& src)
{
    std::cout << "Copy called" << std::endl;
    *this = src;
}

Server& 
Server::operator= (Server& src)
{
	(void) src;
	std::cout << "operator= called" << std::endl;
	return *this;
}

Server::~Server (void)
{
    std::cout << "Destructor called" << std::endl;
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
