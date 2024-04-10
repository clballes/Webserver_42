/*                                                                            */
/* define.hpp                                                                 */
/* mpuig-ma <mpuig-ma@student.42barcelona.com>                                */
/* Mon Jan  8 12:35:17 2024                                                   */

#ifndef _WEBSERV_DEFINE_HPP_
#define _WEBSERV_DEFINE_HPP_

#define webserv_version		1
#define WEBSERV_VERSION		"1.1"
#define EXEC_NAME			"webserv"
#define PROGRAM_NAME		EXEC_NAME
#define WEBSERV_VER			EXEC_NAME"/" WEBSERV_VERSION

#define DEFAULT_CONF		"conf/default.conf"

#define F_AUTOINDEX			0x00010000 
#define METHOD_GET			0x00001000
#define METHOD_POST			0x00000100
#define METHOD_PUT			0x00000010
#define METHOD_DELETE		0x00000001
#define METHOD_HEAD			0x00100000

#define HTTP_GET		0x00000001
#define HTTP_HEAD		0x00000002
#define HTTP_POST		0x00000003
#define HTTP_PUT		0x00000004
#define HTTP_DELETE		0x00000005
#define HTTP_11			0x11

#define LF 012
#define CR 015
#define SP 040

#include "HTTP_status_codes.hpp"

#endif /* !_WEBSERV_DEFINE_HPP_ */
