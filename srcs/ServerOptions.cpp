#include "./inc/ServerOptions.hpp"

// ---------------- INIZIALIZATION FUNCTIONS -----------------------

ServerOptions::ServerOptions()
{
    std::cout << "Constructor called " << std::endl;
}

ServerOptions::ServerOptions(std::string filename) //faig el open i ho carrego tot en una llista
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
        if (!line.empty()) {
            this->listConfig.push_back(line);
        }
    }
    // Print the list
    for (std::list<std::string>::iterator it = listConfig.begin(); it != listConfig.end(); ++it)
    {
        if (std::next(it) != listConfig.end())
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


void ServerOptions::parseConfigFile()
{
    //check tottot
}

std::string ServerOptions::trim(const std::string& str)
{
    std::string::const_iterator it = str.begin();
    std::string::const_iterator it_end = str.end();
    while (it != str.end() && isspace(*it)) {
        ++it;
    }
    while (it_end != str.begin() && isspace(*(it_end - 1))) {
        --it_end;
    }
    return std::string(it, it_end);
}

void ServerOptions::configServer()
{
    std::string input = "   server{   ";

    // Find the position of "server{"
    size_t pos = input.find("server{");

    // Check if "server{" is found
    if (pos != std::string::npos) {
        // Print the substring after "server{" (including possible spaces)
        std::cout << "Words after 'server{' : " << input.substr(pos + 7) << std::endl;
    } else {
        std::cout << "Sequence 'server{' not found in the input string." << std::endl;
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
