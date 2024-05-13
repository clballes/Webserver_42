/*                                                                            */
/* file.hpp                                                                   */
/* mpuig-ma <mpuig-ma@student.42barcelona.com>                                */
/* Wed Apr 10 14:49:41 2024                                                   */

#pragma once

#include <string>
#include <sstream>
// #include "HTTP.hpp" // Include the header file where HTTP is defined

class HTTP;

bool is_regular_file ( const std::string & filename );
bool routeExists ( const std::string & route );
bool can_access_file ( const std::string & filename, int mask );
template<typename T> std::string my_to_string(const T& value);
std::string tolower_string(const std::string& str);
int generate_html( HTTP & http );
std::vector<std::pair<std::string, std::string> > parse_string(const std::string& input);