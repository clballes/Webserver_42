/*                                                                            */
/* log.hpp                                                                  */
/* mpuig-ma <mpuig-ma@student.42barcelona.com>                                */
/* Mon Mar 25 15:11:33 2024                                                   */

#ifndef _LOG_HPP__
#define _LOG_HPP__

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

#define RED		"\033[0;31m"
#define YELLOW	"\033[0;33m"
#define RESET	"\033[0m"
#define ENDL	"\n"

#define DEBUG_TIME \
	char t[10]; \
	std::time_t tt = std::time( 0x0 ); \
	std::strftime( t, 10, "%H:%M:%S", std::localtime( &tt ) ); \
	std::clog << "[" << t << "] ";

#define DEBUG_TIME_EMPTY std::clog << "           ";

// if not debug set should be:
// #define DEBUG(item) ""

#define DEBUG(item) \
{ \
	DEBUG_TIME \
	std::clog \
	<< __FN__ \
	<< " [" << item << "]" \
	<< ENDL; \
}

#define ERROR_TIME(s) \
{ \
	DEBUG_TIME \
	std::clog << s << ENDL; \
}

#define ERROR_EMPTY(s) \
{ \
	DEBUG_TIME_EMPTY \
	std::clog << s << ENDL; \
}

#define ERROR(s) \
{ \
	std::clog << s << ENDL; \
}

#define INFO_TIME(s) \
{ \
	DEBUG_TIME \
	std::clog << s << ENDL; \
}

#define INFO_EMPTY(s) \
{ \
	DEBUG_TIME_EMPTY \
	std::clog << s << ENDL; \
}

#define INFO(s) \
{ \
	std::clog << s << ENDL; \
}

#define WARN(s) \
{ \
	std::clog << YELLOW \
	<< s << RESET << ENDL; \
}

#define LOG(s) { std::clog << s << ENDL; }

#define LOG_BUFFER(str) \
{ \
	char *jakala = (char *) str; \
	size_t it = 1; \
	while ( jakala != 0 && *jakala != 0 ) \
	{ \
		if ( std::isprint( *jakala ) ) { std::clog << *jakala; } \
		else if ( *jakala == 011 ) { std::clog << "\\t"; ++it; } \
		else if ( *jakala == 012 ) { std::clog << "\\n"; ++it; } \
		else if ( *jakala == 015 ) { std::clog << "\\r"; ++it; } \
		if ( it >= 40 ) { std::clog << ENDL; it = 0; } \
		++jakala; \
		++it; \
	} \
	std::clog << ENDL; \
}

#endif /* !_LOG_HPP_ */
