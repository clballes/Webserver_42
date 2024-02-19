/*                                                                            */
/* parse.hpp                                                                  */
/* mpuig-ma <mpuig-ma@student.42barcelona.com>                                */
/* Thu Feb 15 16:50:45 2024                                                   */

#pragma once

#include "Log.hpp"
#include "ServerConf.hpp"
#include <deque>
#include <string>

#include <iostream>
#include <fstream>
#include <sstream>
#include <sys/stat.h>

int count_brackets( std::deque< std::string > & );
bool isRegularFile( const std::string & );
int count_servers( std::deque< std::string > & );

std::string & trim_f( std::string &, int ( *func )( int ) );
void trim( std::string & );

void add_elements( std::string &, std::deque< std::string > & );
