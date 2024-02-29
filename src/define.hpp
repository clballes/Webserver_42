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
#  define WEBSERV_OK		0
# endif /* WEBSERV_OK */

# define DEFAULT_CONF		"conf/default.conf"

#define METHOD_GET 8        //1000
#define METHOD_POST 4       //0100
#define METHOD_PUT 2        //0010
#define METHOD_DELETE 1     //0001

#endif /* define.hpp */
