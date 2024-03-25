/*                                                                            */
/* ServerConf.hpp                                                             */
/* mpuig-ma <mpuig-ma@student.42barcelona.com>                                */
/* Wed Feb 14 17:42:36 2024                                                   */

#pragma once

#include "webserv.hpp"
#include <fstream>
#include <deque>
#include <vector>
#include <map>
#include "Log.hpp"

#define DEFAULT_ERROR_DIR "www"

// class Server;
class ServerConf;

// struct s_configuration_directives
// {
// 	const char *directive_name;
// 	int ( *set_func )( ServerConf &, const char * );
// };

// typedef struct s_configuration_directives t_configuration_directives;

typedef struct s_configuration_directives
{
	const char *directive_name;
	int ( *set_func )( ServerConf &, const char * );

} t_configuration_directives;

class ServerConf
{
	public:
		ServerConf ( void );
		ServerConf ( const ServerConf & );
		ServerConf ( const std::deque< std::string > & );
		~ServerConf ( void );
		// friend class Server;
		
		bool good;
		static void clear ( void ); // crec q no va aqui
		friend std::ostream & operator << ( std::ostream&, const ServerConf & );
		struct sockaddr_in getAddress() const;

	private:
		int set_directives ( const std::deque< std::string > & );

		static int set_listen ( ServerConf &, const char * );
		static int set_root ( ServerConf &, const char * );
		static int set_server_name ( ServerConf &, const char * );
		static int set_error_page ( ServerConf &, const char * );
		static int set_client_body ( ServerConf &, const char * );
		static int set_cgi_param ( ServerConf &, const char * );
		static int set_cgi_pass ( ServerConf &, const char * );
		static int set_allow_methods ( ServerConf &, const char * );
		static int set_index ( ServerConf &, const char * );
		static int set_autoindex ( ServerConf &, const char * );

		static t_configuration_directives _config_directives[];

		struct sockaddr_in			_address;
		std::vector< std::string>	_server_name;
		std::string					_root;
		unsigned int 				_flags;
		std::size_t					_client_max_body_size;
		std::vector< std::string>	_index;
		std::string 				_cgi_param; //nse si guardar en un vector
		std::string					_cgi_pass; 
		std::map<int, std::string>	_error_page;
		
};
