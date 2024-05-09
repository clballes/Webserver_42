/*                                                                            */
/* log.hpp                                                                  */
/* mpuig-ma <mpuig-ma@student.42barcelona.com>                                */
/* Mon Mar 25 15:11:33 2024                                                   */

#ifndef _LOG_HPP__
#define _LOG_HPP__

#include <sys/cdefs.h>
#include <ctime>
#include <iostream>

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

#define _MODE_DEBUG		5
#define _MODE_INFO		4
#define _MODE_NOTICE	3
#define _MODE_WARN		2
#define _MODE_ERROR		1

#define DEBUG(item)		_WEBSERV_LOG( _MODE_DEBUG, item )
#define INFO(item)		_WEBSERV_LOG( _MODE_INFO, item )
#define NOTICE(item)	_WEBSERV_LOG( _MODE_NOTICE, YELLOW << item )
#define WARN(item)		_WEBSERV_LOG( _MODE_WARN, YELLOW << item )
#define ERROR(item)		_WEBSERV_LOG( _MODE_ERROR, RED << item )
#define LOG(item)		{ std::clog << item << RESET << ENDL; }

__BEGIN_DECLS
extern int * __w_verbose_level(void);
#define _webserv_verbose_level (*__w_verbose_level())
__END_DECLS

int _webserv_verbose_level;

#define _WEBSERV_LOG_TIME \
	char t[10]; \
	std::time_t tt = std::time( 0x0 ); \
	std::strftime( t, 10, "%H:%M:%S", std::localtime( &tt ) ); \
	std::clog << "[" << t << "] ";

#define _WEBSERV_LOG(mode,item2log) \
{ \
	int m = mode; \
	if ( m <= _webserv_verbose_level ) \
	{ \
		_WEBSERV_LOG_TIME \
		if ( m == _MODE_DEBUG ) \
		{ std::clog << __FN__ << " [" << item2log << "]"; } \
		else { std::clog << item2log; } \
		std::clog << RESET << ENDL; \
	} \
}
#define LOG_BUFFER(str,color) \
{ \
	std::string::const_iterator it = str.begin(); \
	std::string::size_type counter = 1; \
	while ( it != str.end() ) \
	{ \
		if ( it == str.begin() ) { std::clog << color; } \
		if ( std::isprint( *it ) ) { std::clog << *it; } \
		else if ( *it == '\t' ) { std::clog << "\\t"; ++counter; } \
		else if ( *it == '\n' ) { std::clog << "\\n"; ++counter; } \
		else if ( *it == '\v' ) { std::clog << "\\v"; ++counter; } \
		else if ( *it == '\f' ) { std::clog << "\\f"; ++counter; } \
		else if ( *it == '\r' ) { std::clog << "\\r"; ++counter; } \
		/*if ( counter >= 80 ) { std::clog << ENDL; counter = 0; } */\
		++it; \
		++counter; \
		if ( it == str.end() ) { std::clog << ENDL; } \
	} \
	std::clog << RESET; \
}

#endif /* !_LOG_HPP_ */
