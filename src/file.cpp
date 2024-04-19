/*                                                                            */
/* file.cpp                                                                   */
/* mpuig-ma <mpuig-ma@student.42barcelona.com>                                */
/* Wed Apr 10 14:49:21 2024                                                   */

#include "file.hpp"

bool
is_regular_file( const std::string & filename )
{
	struct stat file_info;

    if ( stat( filename.c_str(), &file_info ) != 0 )
        return ( false );
    return ( S_ISREG( file_info.st_mode ) );
}

bool routeExists(const std::string& route) {
    return access(route.c_str(), 0) == 0;
}
