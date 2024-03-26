/*                                                                            */
/* s_server_conf.hpp                                                          */
/* mpuig-ma <mpuig-ma@student.42barcelona.com>                                */
/* Tue Mar 26 11:04:37 2024                                                   */

#ifndef _T_SERVER_CONF_HPP_
#define _T_SERVER_CONF_HPP_

#include <string>
#include <vector>
#include <map>

struct s_server_conf
{
	std::size_t 				 _flags;
	std::size_t					 _client_max_body_size;
	std::string 				 _cgi_param;
	std::string					 _cgi_pass;
	std::string					 _root;
	std::vector< std::string >	 _server_name;
	std::vector< std::string >	 _index;
	std::map< int, std::string > _error_page;
};

typedef struct s_server_conf t_server_conf;

#endif /* !_T_SERVER_CONF_HPP_ */
