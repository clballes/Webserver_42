/*                                                                            */
/* ParsingServers.hpp                                                         */
/* mpuig-ma <mpuig-ma@student.42barcelona.com>                                */
/* Mon Jan  8 12:35:17 2024                                                   */

#ifndef _WEBSERV_PARSINGSERVERS_HPP_
# define _WEBSERV_PARSINGSERVERS_HPP_
# include <iostream>
# include <list>
# include <vector>

std::string                                 trim_sp(const std::string& input);
std::string                                 trim(const std::string& input);
void                                        addElements(std::string &line,  std::list<std::string>&listConfigig);
int                                         numServers(std::list<std::string>listConfig);
std::vector<std::list<std::string> >        splitServer(int servers, std::list<std::string>listConfig);
int                                         parsingServers(std::string filename, std::list<std::string>&listConfig);
int                                         check_brackets(std::list<std::string>&listConfig);

// void        printArray();
#endif /* ParsingServers.hpp */