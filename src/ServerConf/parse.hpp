/*                                                                            */
/* parse.hpp                                                                  */
/* mpuig-ma <mpuig-ma@student.42barcelona.com>                                */
/* Thu Feb 15 16:50:45 2024                                                   */

#pragma once

#include "Log.hpp"
#include "ServerConf.hpp"
#include <vector>
#include <deque>
#include <string>

#include <iostream>
#include <fstream>
#include <sstream>
#include <sys/stat.h>

int count_brackets ( std::deque< std::string > & );
bool isRegularFile( const std::string & );
int numServers( std::deque< std::string > & );
std::string trim_sp( const std::string & );
std::string trim( const std::string & );
void addElements( std::string &, std::deque< std::string > & );
std::vector< std::deque<std::string> > splitServer( std::deque<std::string> );
