#include <iostream>
#include <string>
#include <list>
#include <vector>
#include <fstream>




std::string trim3(const std::string& input)
{
    std::string result = input;
    size_t lastNonSpace = result.find_last_not_of(" \t");
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

int numServers(std::list<std::string>listConfig) //aixo esta malament si un server falla els guarda els dos ha de sortir si el onfig esta malament
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

// void splitServer() // no utilitzar corchetes i tuilitzar el metode i fer servir referenceis i no allocar memoria
// {
//     unsigned long int i = this->servers;
//     std::list<std::string>::iterator it = this->listConfig.begin();
//     while (it != this->listConfig.end() && i != 0) {
//     if (*it == "server")
//         {
//             std::list<std::string>* subList = new std::list<std::string>;
//             if (i > this->arrayOfLists.size()) {
//                 this->arrayOfLists.resize(i, nullptr);
//             }
//             this->arrayOfLists[i - 1] = subList;
//             ++it;
//             while (it != this->listConfig.end() && *it != "server") {
//                 arrayOfLists[i - 1]->push_back(*it);
//                 ++it;
//             }
//             if (it != this->listConfig.end() && *it == "server") {
//                 --i;
//             }
//         }
//     }
// }

void parsingBrackets(std::string &line, std::list<std::string>&listConfig)
{
    size_t pos_open = line.find("{");
    size_t pos_closed = line.find("}");
    if (!line.empty() && line[0] != '#' && pos_open == std::string::npos && pos_closed == std::string::npos) {
        line = trim3(line);
        listConfig.push_back(line);
    }
    else if (pos_open != std::string::npos){
        if (pos_open == 0 && line.length() != 1)
        {
            std::string substring = line.substr(pos_open + 1);
            listConfig.push_back("{");
            substring = trim3(substring);
            listConfig.push_back(substring);
        }
        else if (pos_open != 0)
        {
            std::string substring = line.substr(0, pos_open);
            substring = trim3(substring);
            listConfig.push_back(substring);
            listConfig.push_back("{");
        }
        else
            listConfig.push_back("{");
    }
    else if(pos_closed != std::string::npos)
    {
        if (pos_closed == 0 && line.length() != 1)
        {
            std::string substring = line.substr(pos_closed + 1);
            listConfig.push_back("}");
            listConfig.push_back(trim3(substring));
        }
        else if (pos_closed != 0)
        {
            std::string substring = line.substr(0, pos_closed);
            listConfig.push_back(trim3(substring));
            listConfig.push_back("}");
        }
        else
            listConfig.push_back("}");
    }
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

void ParsingServers(std::string filename, std::list<std::string>&listConfig)
{
    std::ifstream configFile(filename);
    std::string line;
    if (!configFile.is_open()){
        std::cerr << "ERROR OPENING FILE!" << std::endl;
    }
    while (std::getline(configFile, line))
    {
        line = trim2(line);
        parsingBrackets(line, listConfig);
    }
    // //print list
    // std::list<std::string>::iterator it = listConfig.begin();
    // for(; it != listConfig.end(); ++it)
    // {
    //     std::cout << *it << std::endl;
    // }
    //CHECK HOW MANY SERVERS WE HAVE, REGARDING BRAKETS
    int servers = numServers(listConfig);
    if(!servers)
    {
        std::cerr << "NO SERVERS TO EXECUTE THE PROGRAM, SYNTAX ERR" << std::endl;
    }
    std::cout << servers << std::endl;
    // // split the list in array without server directive but with brackets
    // splitServer();
    // printArray();
}