#include "./inc/ServerOptions.hpp"

// ---------------- INIZIALIZATION FUNCTIONS -----------------------

ServerOptions::ServerOptions()
{
    // std::cout << "Constructor called " << std::endl;
}

//Faig el open i ho carrego tot en una llista trimming spaces tabs x davant i darrere, Printejo la llista final

ServerOptions::ServerOptions(std::list<std::string>*)
{
    std::cout << "hola" << std::endl;
}


ServerOptions::ServerOptions(std::string filename)
{
    std::cout << "Constructor called with " << filename << std::endl;
    std::ifstream configFile(filename);
    std::string line;
    if (!configFile.is_open()){
        throw FailOpen();
    }
    while (std::getline(configFile, line))
    {
        line = trim(line);
        if (!line.empty() && line[0] != '#') {
            this->listConfig.push_back(line);
        }
    }
    // Print the list
    for (std::list<std::string>::iterator it = listConfig.begin(); it != listConfig.end(); ++it)
    {
        std::cout << *it << std::endl;
    }
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

std::string ServerOptions::trim(const std::string& input)
{
    std::string result;

    bool leadingSpacesOrTabs = true;

    for (std::size_t i = 0; i < input.length(); ++i) {
        if (input[i] == ' ' || input[i] == '\t') {
            if (!leadingSpacesOrTabs) {
                result += ' ';
                leadingSpacesOrTabs = true;
            }
        } else {
            result += input[i];
            leadingSpacesOrTabs = false;
        }
    }
    return result;
}

void ServerOptions::parseConfigFile()
{
    for (std::list<std::string>::iterator it = listConfig.begin(); it != listConfig.end(); ++it) {
        // std::cout << *it << std::endl;
    }
}

bool ServerOptions::configServer(const std::string& serverConfig)
{
    std::size_t pos = serverConfig.find("server");

    if (pos != std::string::npos) {
        std::cout << "Server found " << pos << "la string es: " << serverConfig << std::endl;
        return true;
    } else {
        std::cout << "Sequence '{' not found in the input string." << std::endl;
        return false;
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
