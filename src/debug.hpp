/*                                                                            */
/* debug.hpp                                                                  */
/* mpuig-ma <mpuig-ma@student.42barcelona.com>                                */
/* Mon Mar 25 15:11:33 2024                                                   */

#pragma once

#include <iostream>
#include <ctime>

// TODO: define DEBUG to nothing if not -DDEBUG
// TODO: implement hierarchy INFO -> WARN -> ERROR
// nginx's: debug | info | notice | warn | error | crit | alert | emerg

#ifdef PRETTY
#define __FN__ __PRETTY_FUNCTION__
#else
#define __FN__ __FUNCTION__
#endif

#define DEBUG_TIME \
	char t[10]; \
	std::time_t tt = std::time( 0x0 ); \
	std::strftime( t, 10, "%H:%M:%S", std::localtime( &tt ) ); \
	std::clog << "[" << t << "] ";

#define DEBUG_TIME_EMPTY std::clog << "           ";

#define DEBUG(item) \
{ \
	DEBUG_TIME \
	std::clog \
	<< __FN__ \
	<< " [" << item << "]" \
	<< std::endl; \
}

#define ERROR_TIME(s) \
{ \
	DEBUG_TIME \
	std::clog << s << std::endl; \
}

#define ERROR_EMPTY(s) \
{ \
	DEBUG_TIME_EMPTY \
	std::clog << s << std::endl; \
}

#define ERROR(s) \
{ \
	std::clog << s << std::endl; \
}

#define INFO_TIME(s) \
{ \
	DEBUG_TIME \
	std::clog << s << std::endl; \
}

#define INFO_EMPTY(s) \
{ \
	DEBUG_TIME_EMPTY \
	std::clog << s << std::endl; \
}

#define INFO(s) \
{ \
	std::clog << s << std::endl; \
}
