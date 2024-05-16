//                                                                             /
// t_request.hpp                                                               /
// mpuig-ma <mpuig-ma@student.42barcelona.com>                                 /
// Thu May 16 15:43:27 2024                                                    /

#ifndef T_REQUEST_HPP_
#define T_REQUEST_HPP_

#include <sys/stat.h>
#include <string>
#include "t_http_method.hpp"
#include "t_headers.hpp"

typedef struct s_request
{
	int 			http_version;
	t_http_method *	method;
	std::string		host;
	std::string		target;
	std::string		query;
	std::string		body;
	std::string		file;
	struct stat		file_info;
	t_headers		headers;

} t_request;

#endif // ! T_REQUEST_HPP_
