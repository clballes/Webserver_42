/*                                                                            */
/* ServerConf.cpp                                                             */
/* clballes <clballes@student.42barcelona.com>                                */
/* Mon Jan  8 12:39:29 2024                                                   */

#include "ServerConf.hpp"

// ---------------- INIZIALIZATION FUNCTIONS -----------------------
ServerConf::ServerConf (void)
{
    std::cout << "Constructor called " << std::endl;
}

ServerConf::ServerConf (ServerConf& src)
{
    std::cout << "Copy called" << std::endl;
    *this = src;
}

ServerConf& 
ServerConf::operator= (ServerConf& src)
{
	// this->map = src.map;
	(void) src;
	std::cout << "operator= called" << std::endl;
	return *this;
}

ServerConf::~ServerConf (void)
{
    std::cout << "Destructor called" << std::endl;
}

// ------------------------- MEMBER FUNCTIONS -----------------------------------

void
ServerConf::ServerConfList (std::string filename)
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
            this->listServerConf.push_back(std::string(it, line.end()));
        }
    }

    // Print the list
    // for (std::list<std::string>::iterator it = listServerConf.begin(); it != listServerConf.end(); ++it)
    // {
    //     if (std::next(it) != listServerConf.end())
    //         std::cout << *it << std::endl;
    // }

    configFile.close();
    parseServerConf(); //parseamos configuration
}

void
ServerConf::parseServerConf (void)
{
    std::list<std::string>::iterator it;
		
	it = std::find(this->listServerConf.begin(), this->listServerConf.end(), "server  {");
    if (it != this->listServerConf.end())
	{
        std::cout << "Found 'server' in the list!" << std::endl;
    }
	else
	{
        std::cout << "'server' not found in the list." << std::endl;
    }
}



// ------------------------- EXCEPTION FUNCTIONS -----------------------------------

const char*
ServerConf::FailOpen::what(void) const throw()
{
    return "Error: Cannot open the file, file not found.";
}
