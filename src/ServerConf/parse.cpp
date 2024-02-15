/*                                                                            */
/* parse.cpp                                                                  */
/* mpuig-ma <mpuig-ma@student.42barcelona.com>                                */
/* Thu Feb 15 16:50:45 2024                                                   */

#include "Log.hpp"
#include "ServerConf.hpp"
#include <vector>
#include <deque>
#include <string>

int
ServerConf::pre_parse ( std::deque< std::string > & content )
{
	(void) content;

	return ( EXIT_SUCCESS );
}
