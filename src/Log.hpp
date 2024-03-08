/*                                                                            */
/* Log.hpp                                                                    */
/* mpuig-ma <mpuig-ma@student.42barcelona.com>                                */
/* Thu Feb 15 12:46:20 2024                                                   */

#pragma once

#include <iostream>

#define LOG(s) { std::clog << s << std::endl; }

#define LOG_BUFFER(str) \
{ \
	char *jakala = (char *) str; \
	std::clog << "\""; \
	while ( jakala != 0 && *jakala != 0 ) \
	{ \
		if ( std::isprint( *jakala ) ) { std::clog << *jakala; } \
		else if ( *jakala == 011 ) { std::clog << "\\t"; } \
		else if ( *jakala == 012 ) { std::clog << "\\n"; } \
		else if ( *jakala == 015 ) { std::clog << "\\r"; } \
		++jakala; \
	} \
	std::clog << "\"" << std::endl; \
}
