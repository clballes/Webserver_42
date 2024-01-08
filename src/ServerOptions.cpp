#include "./inc/ServerOptions.hpp"

// ---------------- INIZIALIZATION FUNCTIONS -----------------------

ServerOptions::ServerOptions()
{
    std::cout << "Constructor called " << std::endl;
}

//assign la sublist en la llista top
ServerOptions::ServerOptions(std::list<std::string>* list)
{
    this->listConfig.assign(list->begin(), list->end());
}

ServerOptions::ServerOptions(ServerOptions & src)
{
    std::cout << "Copy called" << std::endl;
    *this = src;
}

// ServerOptions & ServerOptions::operator=(ServerOptions & src)
// {
//     this->map = src.map;
//     std::cout << "Operator called" << std::endl;
//     return *this;
// }

ServerOptions::~ServerOptions()
{
    std::cout << "Destructor called" << std::endl;
}

// ------------------------- MEMBER FUNCTIONS -----------------------------------

//he tret els brackets del server
void ServerOptions::parseConfigFile() //contar els bacekts
{
    //  std::list<std::string>::iterator it2;
    // for (it2 = listConfig.begin(); it2 != listConfig.end(); ++it2) {
    //     std::cout << *it2 << std::endl;
    // }

    std::list<std::string>::iterator it = listConfig.begin();
    std::list<std::string>::iterator itEnd = listConfig.end();
        std::advance(itEnd, -1);  // Move the iterator to the last element

    if (*itEnd == "}" && *it == "{") {
            listConfig.erase(itEnd);
            listConfig.erase(it);
    }
    else {
        std::cout << "'}' not found in the list, error in the server directive" << std::endl;
    }

}

void ServerOptions::configLocation()
{
}

// ------------------------- EXCEPTION FUNCTIONS -----------------------------------

const char * ServerOptions::FailOpen::what() const throw()
{
    return "Error: Cannot open the file, file not found.";
}
