/*                                                                            */
/* parse.cpp                                                                  */
/* mpuig-ma <mpuig-ma@student.42barcelona.com>                                */
/* Wed Feb 14 17:22:43 2024                                                   */

#include "parse.hpp"
#include "ServerConf.hpp"

int
parse_configuration ( std::ifstream & file )
{
	if ( ServerConf::add( file ) == EXIT_FAILURE )
		return ( EXIT_FAILURE );

	return ( EXIT_SUCCESS );
}
