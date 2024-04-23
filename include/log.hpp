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
#define GREEN	"\033[0;32m"
#define YELLOW	"\033[0;33m"
#define BLUE	"\033[0;34m"
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

#define LOG(s) { std::clog << s << RESET << ENDL; }

#define LOG_BUFFER(str,color) \
{ \
	std::string::const_iterator it = str.begin(); \
	std::string::size_type counter = 1; \
	while ( it != str.end() ) \
	{ \
		if ( it == str.begin() ) { std::clog << color; } \
		if ( std::isprint( *it ) ) { std::clog << *it; } \
		else if ( *it == '\t' ) { std::clog << "\\t"; ++it; } \
		else if ( *it == '\n' ) { std::clog << "\\n"; ++it; } \
		else if ( *it == '\v' ) { std::clog << "\\v"; ++it; } \
		else if ( *it == '\f' ) { std::clog << "\\f"; ++it; } \
		else if ( *it == '\r' ) { std::clog << "\\r"; ++it; } \
		/*if ( counter >= 80 ) { std::clog << ENDL; counter = 0; } */\
		++it; \
		++counter; \
		if ( it == str.end() ) { std::clog << ENDL; } \
	} \
	std::clog << RESET; \
}

#endif /* !_LOG_HPP_ */
