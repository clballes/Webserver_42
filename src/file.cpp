/*                                                                            */
/* file.cpp                                                                   */
/* mpuig-ma <mpuig-ma@student.42barcelona.com>                                */
/* Wed Apr 10 14:49:21 2024                                                   */

#include "file.hpp"
#include <unistd.h>
#include <iostream>
#include <sys/stat.h>

bool
is_regular_file( const std::string & filename )
{
	struct stat file_info;

    if ( stat( filename.c_str(), &file_info ) == -1 )
        return ( false );
    return ( S_ISREG( file_info.st_mode ) );
}

bool
can_access_file ( const std::string & filename, int mask )
{
	if ( is_regular_file( filename ) == true
			&& access( filename.c_str(), mask ) == 0 )
            return ( true );
    return ( false ); 
}

bool routeExists( const std::string & route )
{
    return ( access( route.c_str(), 0 ) == 0 );
}
