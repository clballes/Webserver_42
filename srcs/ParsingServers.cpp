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
    // Trim trailing spaces and tabs
    size_t lastNonSpace = result.size();
    while (lastNonSpace > 0 && (result[lastNonSpace - 1] == ' ' || result[lastNonSpace - 1] == '\t')) {
        --lastNonSpace;
    }
    // Erase trailing spaces and tabs
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

void        addElements(std::string &line,  std::list<std::string>&listConfig);

void addBrackets(std::string &line, std::list<std::string>&listConfig, size_t pos,  std::string bracket)
{
    if (pos == 0 && line.length() != 1){
        std::string substring = line.substr(pos + 1);
        listConfig.push_back(bracket);
        listConfig.push_back(trim_sp(substring));
    }
    else if (pos != 0){
        // std::cout << "2nd: " << line << std::endl;
        std::string substring = line.substr(0, pos);
        if (pos != (line.length() - 1))
        {
            std::string sub2 = line.substr(pos + 1, line.length());
            // std::cout << "------------- sub2 is: -----"  << sub2 << std::endl;
        }
        listConfig.push_back(trim_sp(substring));
        listConfig.push_back(bracket);
    }
    else if(pos == 0 && line.length() == 1){
      listConfig.push_back(bracket);
    }
    else
    {
        std::cerr << "Error in the directive, brackets parsing list" << std::endl;
    }
}

// void    addElements(std::string &line, std::list<std::string>&listConfig)
// {
//     size_t start = 0;
//     size_t end;
//     end = line.find_first_of("{}", start);
//     while (end != std::string::npos || start == 0)
//     {
//         std::string sub = line.substr(start, end);
//         listConfig.push_back(sub);
//         start = end + 1;
//         end = line.find_first_of("{}", start);
//     }

// }
/*
    if (!line.empty() && line[0] != '#'){
        listConfig.push_back(trim_sp(line));
    }
    else if (pos_open != std::string::npos){
        addBrackets(line, listConfig, pos_open, "{");
    }
    else if(pos_closed != std::string::npos){
        addBrackets(line, listConfig, pos_closed, "}");
    }
    */


void    addElements(std::string &line, std::list<std::string>&listConfig)
{
    size_t pos_open = line.find("{");
    size_t pos_closed = line.find("}");
    if (!line.empty() && line[0] != '#'){
        listConfig.push_back(trim_sp(line));
    }
    else if (pos_open != std::string::npos){
        addBrackets(line, listConfig, pos_open, "{");
    }
    else if(pos_closed != std::string::npos){
        addBrackets(line, listConfig, pos_closed, "}");
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

int    ParsingServers(std::string filename, std::list<std::string>&listConfig)
{
    std::ifstream configFile(filename);
    std::string line;
    if (!configFile.is_open()){
        std::cerr << "ERROR OPENING FILE!" << std::endl;
    }

    while (std::getline(configFile, line))
    {
        line = trim(line);
        addElements(line, listConfig);
    }
    // std::list<std::string>::iterator it = listConfig.begin();
    // for(; it != listConfig.end(); ++it)
    // {
    //     std::cout << *it << std::endl;
    // }
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
    //     std::list<std::string>::iterator it = listConfig.begin();
    //     for(; it != listConfig.end(); ++it)
    //     {
    //         std::cout << *it << std::endl;
    //     }
    //     std::cerr << "Syntax error: server directive syntax" << std::endl;
    //     return 1;
    // }
    return 0;
}