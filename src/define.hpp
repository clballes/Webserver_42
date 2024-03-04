/*                                                                            */
/* define.hpp                                                                 */
/* mpuig-ma <mpuig-ma@student.42barcelona.com>                                */
/* Mon Jan  8 12:35:17 2024                                                   */

#ifndef _WEBSERV_DEFINE_HPP_
# define _WEBSERV_DEFINE_HPP_

# define EXEC_NAME			"webserv"
# define PROGRAM_NAME		EXEC_NAME

# define webserv_version	1
# define WEBSERV_VERSION	"1.0"
# define WEBSERV_VER		EXEC_NAME"/" WEBSERV_VERSION

# ifndef WEBSERV_OK
#  define WEBSERV_OK		
# endif /* WEBSERV_OK */

# define DEFAULT_CONF		"conf/default.conf"
# define DEFAULT_ERR    	"conf/default_err.conf"

#define F_AUTOINDEX     0x00010000 
#define  METHOD_GET     0x00001000
#define METHOD_POST     0x00000100
#define METHOD_PUT      0x00000010
#define METHOD_DELETE   0x00000001

#endif /* define.hpp */
