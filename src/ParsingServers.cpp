#include <iostream>
#include <string>
#include <list>
#include <vector>
#include <fstream>
#include <iostream>
#include <fstream>
#include <sstream>

// ------- TRIM FUNCTIONS -------
std::string trim_sp(const std::string& input)
{
    std::string result = input;
    size_t lastNonSpace = result.size();
    while (lastNonSpace > 0 && (result[lastNonSpace - 1] == ' ' || result[lastNonSpace - 1] == '\t')) {
        --lastNonSpace;
    }
    if (lastNonSpace < result.size()) {
        result.erase(lastNonSpace);
    }
    return result;
}

std::string trim(const std::string& input)
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

// ------- BASYC SYNTAX FUNCTIONS -------
int numServers(std::list<std::string>listConfig)
{
    int count = 0;
    for (std::list<std::string>::iterator it = listConfig.begin(); it != listConfig.end(); ++it)
    {
        if (it->find("server") != std::string::npos)
        {
            if (it->length() == 6 && (*it)[it->length()] == '\0'){
                count++;
            }
            else if ((*it)[6] != '_')
            {
                return 0;
            }
        }
    }
    return count;
}

int    check_brackets(std::list<std::string>&listConfig)
{
    int openBrace = 0;
    int closedBrace = 0;
    std::list<std::string>::iterator it = listConfig.begin();
    for(; it != listConfig.end(); ++it)
    {
        if (*it == "{")
        {
            openBrace++;
        }
        else if (*it == "}")
        {
            closedBrace++;
        }
    }
    if (closedBrace != openBrace)
        return 1;
    return 0;
}

// ----- SPLITSERVER ----------------
std::vector<std::list<std::string> > splitServer(int servers, std::list<std::string>listConfig){
    std::vector<std::list<std::string> >arrayOfLists(servers);
    std::list<std::string>::iterator it = listConfig.begin();
    while (it != listConfig.end() && servers!= 0) {
    if (*it == "server")
        {
            std::list<std::string> subList;
            arrayOfLists[servers- 1] = subList;
            ++it;
            while (it != listConfig.end() && *it != "server") {
                arrayOfLists[servers- 1].push_back(*it);
                ++it;
            }
            if (it != listConfig.end() && *it == "server") {
                --servers;
            }
        }
    }
    return arrayOfLists;
}

// ----- CREATE LIST FUNCTIONS  ----- Create the list seaprating each element by brackets in a new line
void    addElements(std::string &line, std::list<std::string>&listConfig)
{
    size_t start = 0;
    size_t end;
    end = line.find_first_of("{}", start);
    if (end == std::string::npos)
    {
        listConfig.push_back(line);
        return ;
    }
    else
    {
        while (end != std::string::npos || start == 0)
        {
            std::string sub = line.substr(start, end - start);
            if (!sub.empty() || sub.length() != 0)
            {
                listConfig.push_back(trim_sp(sub));
            }
            start = end + 1;
            end = line.find_first_of("{}", start);
            if (end == std::string::npos)
                listConfig.push_back(trim_sp(line.substr(start - 1, 1)));
        }
    }
}

int    parsingServers(std::string filename, std::list<std::string>&listConfig)
{
    std::ifstream configFile(filename);
    std::string line;
    if (!configFile.is_open()){
        std::cerr << "ERROR OPENING FILE!" << std::endl;
    }
    while (std::getline(configFile, line))
    {
        line = trim(line);
        if (line[0] != '#' && line.length()!= 0)
        {
            addElements(line, listConfig);
        }
    }
    std::list<std::string>::iterator it = listConfig.begin();
    std::cout << "PRINTING LIST -------" << std::endl;
    for(; it != listConfig.end(); ++it)
    {
        std::cout << *it << std::endl;
    }
    // CHECK directives are closed
    // int brackets = check_brackets(listConfig);
    // if (brackets)
    // {
    //     std::cout << brackets << std::endl;
    //     std::cerr << "Syntax Error: Mising brackets" << std::endl;
    //     return 1;
    // }
    // // CHECK how many servers we have and that is correct syntax
    // int servers = numServers(listConfig);
    // if(!servers)
    // {
        // std::list<std::string>::iterator it = listConfig.begin();
        // for(; it != listConfig.end(); ++it)
        // {
        //     std::cout << *it << std::endl;
        // }
    //     std::cerr << "Syntax error: server directive syntax" << std::endl;
    //     return 1;
    // }
    return 0;
}

// void   printArray()
// {
//     for (unsigned long int i = 0; i < this->servers; ++i) {
//         std::cout << "-------------Sublist------------" << i + 1 << std::endl;
//         for (std::list<std::string>::iterator sublistIt = arrayOfLists[i]->begin(); sublistIt != arrayOfLists[i]->end(); ++sublistIt) {
//             std::cout << *sublistIt << std::endl;
//         }
//     }
// }