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
#define DEFAULT_ERROR_DIR   "www"

// The following define(s) are meant to be
// used for setting and checking server flags.
// For example, with the allow_methods directive.
// The following define(s) are meant to be
// used with a t_http_method.

#define F_AUTOINDEX			0x00010000 
#define HTTP_GET			0x00001000
#define HTTP_POST			0x00000100
#define HTTP_PUT			0x00000010
#define HTTP_DELETE			0x00000001
#define HTTP_HEAD			0x00100000
#define HTTP_10				0x10
#define HTTP_11				0x11

#ifndef LF
#define LF					012
#endif

#ifndef CR
#define CR					015
#endif

#ifndef SP
#define SP 					040
#endif

#define CGI_TIMEOUT			8
#define REQ_TIMEOUT			30

#include "HTTP_status_codes.hpp"

#endif /* !_WEBSERV_DEFINE_HPP_ */
