#include "Config.hpp"

// ---------------- INIZIALIZATION FUNCTIONS -----------------------
Config::Config()
{
    std::cout << "Constructor called " << std::endl;
}

Config::Config(Config & src)
{
    std::cout << "Copy called" << std::endl;
    *this = src;
}

// Config & Config::operator=(Config & src)
// {
//     this->map = src.map;
//     std::cout << "Operator called" << std::endl;
//     return *this;
// }

Config::~Config()
{
    std::cout << "Destructor called" << std::endl;
}

// ------------------------- MEMBER FUNCTIONS -----------------------------------

void Config::ConfigList(std::string filename)
{
    this->filename = filename;
    std::ifstream configFile(filename);
    std::string line;
    if (!configFile.is_open()){
        throw FailOpen();
    }
    while (getline(configFile, line))
    {
        std::string::iterator it = line.begin();
        while (it != line.end() && isspace(*it)) {
            ++it;
        }
        if (!line.empty()) {
            this->listConfig.push_back(std::string(it, line.end()));
        }
    }
    // Print the list
    // for (std::list<std::string>::iterator it = listConfig.begin(); it != listConfig.end(); ++it)
    // {
    //     if (std::next(it) != listConfig.end())
    //         std::cout << *it << std::endl;
    // }
    configFile.close();
    parseConfig(); //parseamos configuration
}

void Config::parseConfig()
{
    std::list<std::string>::iterator it = std::find(this->listConfig.begin(), this->listConfig.end(), "server  {");
    if (it != this->listConfig.end()) {
        std::cout << "Found 'server' in the list!" << std::endl;
    } else {
        std::cout << "'server' not found in the list." << std::endl;
    }
}



// ------------------------- EXCEPTION FUNCTIONS -----------------------------------

const char * Config::FailOpen::what() const throw()
{
    return "Error: Cannot open the file, file not found.";
}
