// #include "./inc/Server.hpp"
#include "./inc/ServerOptions.hpp"
#include <list>
#include <string>

std::list<std::string> listConfig;


std::string eraseTrailingSpaces(const std::string& input)
{
    std::string result = input;

    // Find the position of the last non-space character
    size_t lastNonSpace = result.find_last_not_of(" \t");

    // If there are trailing spaces, erase them
    if (lastNonSpace != std::string::npos && lastNonSpace < result.length() - 1) {
        result.erase(lastNonSpace + 1);
    }

    return result;
}
std::string trim2(const std::string& input)
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

// void checkBracelets(std::list<std::string>listConfig)
// {

// }

int    check_servers(std::string filename)
{
    int count = 0;
    std::ifstream configFile(filename);
    std::string line;
    if (!configFile.is_open()){
        std::cerr << "ERROR OPENING FILE!" << std::endl;
    }
    while (std::getline(configFile, line))
    {
        line = trim2(line);
    //     if (!line.empty() && line[0] != '#') {
    //         listConfig.push_back(line);
    //     }
    // }
    size_t pos = line.find("{");
    if (!line.empty() && line[0] != '#' && pos == std::string::npos) {
        listConfig.push_back(line);
    }
    else if (pos != std::string::npos)
    {
        std::cout << "posicion: "<< pos << std::endl;
        if (pos == 0 && line.length() != 1)
        {
            // std::cout << line << std::endl;
            std::string substring = line.substr(pos + 1);
            listConfig.push_back("{");
            substring = trim2(substring);
            listConfig.push_back(substring);
            // std::cout << "posicion: "<< substring << std::endl;
        }
        else if (pos != 0)
        {
            std::string substring = line.substr(0, pos);
            // std::cout << "substr: "<< substring << std::endl;
            substring = trim2(substring);
            listConfig.push_back(substring);
            listConfig.push_back("{");
        }
        else
            // std::cout << "line is: " << line << std::endl;
            listConfig.push_back("{");
    }
    }
    //CHECK HOW MANY SERVERS WE HAVE, REGARDING BRAKETS
    for (std::list<std::string>::iterator it = listConfig.begin(); it != listConfig.end(); ++it)
    {
        bool hasError = false;
        if (it->find("server") != std::string::npos)
        {
            if (it->length() == 6)
            {
                count++;
            //     std::list<std::string>subList;
            // //  std::list<std::string> subList;
            //     for (; it != listConfig.end() && it->find("server ") != std::string::npos; ++it) {
            //         subList.push_back(*it);
            //     }
            //     // Process the subList as needed
            //     for (std::list<std::string>::iterator itSub = subList.begin(); itSub != subList.end(); ++itSub) {
            //         std::cout << "sublist: " << *itSub << std::endl;
            //     }
            }else {
                for (size_t i = 6; i < it->length() && !hasError; i++) {
                    if ((*it)[i] != ' ' && (*it)[i] != '{' && (*it)[i] != '\0') {
                        hasError = true;
                    }
                }
                if (!hasError)
                {
                    count++;
                } 
            }
        }
    }
    // Print the list
    for (std::list<std::string>::iterator it = listConfig.begin(); it != listConfig.end(); ++it)
    {
        std::cout << *it << std::endl;
    }    
    return count;
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        std::cerr << "ERROR!" << std::endl;
        return 1;
    }
    else
    {
        int server = check_servers(argv[1]);
        std::cout << "server num: " << server << std::endl;
        try
        {
            // while(server)
            // {
            //     ServerOptions serverConf(argv[1]);
            //     serverConf.parseConfigFile();
            // }
            // config.ConfigServer(argv[1]);
            // config.configLocation();

        }
        catch(const std::exception & e)
        {
            std::cout << e.what() << std::endl;
        }
    }
    return 0;
}