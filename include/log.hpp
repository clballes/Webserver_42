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

#define _MODE_LOG		4
#define _MODE_INFO		3
#define _MODE_NOTICE	2
#define _MODE_WARN		1
#define _MODE_ERROR		0

#define INFO(item)		_WEBSERV_LOG( _MODE_INFO, item )
#define NOTICE(item)	_WEBSERV_LOG( _MODE_NOTICE, YELLOW << item )
#define WARN(item)		_WEBSERV_LOG( _MODE_WARN, YELLOW << item )
#define ERROR(item)		_WEBSERV_LOG( _MODE_ERROR, RED << item )
#define LOG(item)		_WEBSERV_LOG( _MODE_LOG, item )

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
		if ( m != _MODE_LOG ) { _WEBSERV_LOG_TIME } \
		std::clog << item2log << RESET << ENDL; \
	} \
}

#endif /* !_LOG_HPP_ */
