/*                                                                            */
/* file.hpp                                                                   */
/* mpuig-ma <mpuig-ma@student.42barcelona.com>                                */
/* Wed Apr 10 14:49:41 2024                                                   */

#pragma once

#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>

#include <string>
#include <vector>
#include <fstream>

#include "string.hpp"
#include "t_headers.hpp"
#include "t_request.hpp"
#include "HTTP_status_codes.hpp"
#include "log.hpp"

bool is_regular_file ( const std::string & filename );
bool routeExists ( const std::string & route );
bool can_access_file ( const std::string & filename, int mask );

int load_file ( const std::string & filename, std::string & dst );
int autoindex ( const t_request & request, std::string & dst );
int generate_html ( const t_request & request, std::string & dst );
