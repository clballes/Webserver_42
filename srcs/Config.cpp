#include "./inc/Config.hpp"

// ---------------- INIZIALIZATION FUNCTIONS -----------------------

// Config::Config() : server() initialize the server object idk preguntar martins 
// {
//     // Constructor initializes default values for the Server instance
//     server.server_name = "DefaultServer";
//     server.listen = "127.0.0.1:8080";
//     server.root = "/var/www/html";
//     server.index = "index.html";
//     server.allow_methods = "GET,POST";

//     std::cout << "Constructor called " << std::endl;
// }
Config::Config()
{
    // Constructor initializes default values for the Server instance
    // server.server_name = "DefaultServer";
    // server.listen = "127.0.0.1:8080";
    // server.root = "/var/www/html";
    // server.index = "index.html";
    // server.allow_methods = "GET,POST";
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

std::string Config::trim(const std::string& str)
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

void Config::ConfigServer(std::string filename)
{
    this->filename = filename;
    std::ifstream configFile(filename);
    std::string line;
    if (!configFile.is_open()){
        throw FailOpen();
    }
    while (std::getline(configFile, line))
    {
        if (line == "	location /put_test {") //ara nomes per tenir el server parseado, no ho farem aixi
        {
            break;
        }
        std::istringstream iss(line);
        std::string firstWord;
        iss >> firstWord;
        std::string restOfLine;
        std::getline(iss, restOfLine);
        restOfLine = trim(restOfLine);
        this->mapConfigServer[firstWord] = restOfLine;
    }
    // Display the map contents
    // for (std::map<std::string, std::string>::iterator it = this->mapConfigServer.begin(); it != this->mapConfigServer.end(); ++it) {
    //     std::cout << "Key: " << it->first << ", Value: " << it->second << std::endl;
    // }
    configFile.close();
    parseConfig();
}

void Config::configLocation()
{
    std::string line;
    std::ifstream configFile(this->filename);

    Location loc;
    while (std::getline(configFile, line))
    {
        // if (line.empty() || line.find('#') != std::string::npos)
            // continue;
        //  if (line.find("location /put_test {") != std::string::npos)
        //  {
        //     std::string key;
        //         configFile >> key;
        //         std::cout << "key: "<<key << std::endl;

        //     while (std::getline(configFile, line))
        //     {
        //         std::string key;
        //         configFile >> key;
        //         std::cout << "key: "<<key << std::endl;
        //     }
            // Read data from the configFile and populate the Location structure
                // if (loc.path == "}")
                // {
                //     // break;
                // }
                // if (key == "allow_methods")
                // {
                //     configFile >> std::ws; // Consume any leading whitespaces
                //     std::getline(configFile, loc.*(&Location::allow_methods + key == "allow_methods" ? &Location::allow_methods : &Location::path), '\n');
                // }
            server.Locations.push_back(loc);
            }

        }

void Config::parseConfig()
{
    //hauriem de mirar q pasa si no tenim nom o server etc tirar error com locos i despres, omplir el objecte de server
    populateServer();
    // populateLocation();
}

void Config::populateServer()
{
    this->server.server_name = this->mapConfigServer["server_name"];
    this->server.listen = this->mapConfigServer["listen"];
    this->server.root = this->mapConfigServer["root"];
    this->server.index = this->mapConfigServer["index"];
    this->server.allow_methods = this->mapConfigServer["allow_methods"];
}

const Server& Config::getServer() const
{
    return server;
}

// ------------------------- EXCEPTION FUNCTIONS -----------------------------------
const char * Config::FailOpen::what() const throw()
{
    return "Error: Cannot open the file, file not found.";
}





    // -------- TREBALLAR AMB LLISTES ------------
    // while (getline(configconfigFile, line))
    // {
    //     std::string::iterator it = line.begin();
    //     while (it != line.end() && isspace(*it)) {
    //         ++it;
    //     }
    //     if (!line.empty()) {
    //         this->listConfig.push_back(std::string(it, line.end()));
    //     }
    // }
    // // Print the list
    // for (std::list<std::string>::iterator it = listConfig.begin(); it != listConfig.end(); ++it)
    // {
    //     if (std::next(it) != listConfig.end())
    //         std::cout << *it << std::endl;
    // }