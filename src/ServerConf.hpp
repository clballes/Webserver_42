/*                                                                            */
/* ServerConf.hpp                                                             */
/* mpuig-ma <mpuig-ma@student.42barcelona.com>                                */
/* Wed Feb 14 17:42:36 2024                                                   */

#pragma once

#include "Server.hpp"
#include <fstream>
#include <deque>
#include <vector>
#include <map>

class Server;
class ServerConf;

struct s_configuration_directives
{
	const char *directive_name;
	int ( *set_func )( ServerConf &, const char * );
};

typedef struct s_configuration_directives t_configuration_directives;

class ServerConf
{
	public:

		friend class Server;
		
		bool good;
		static std::deque< const ServerConf * >instances;
	
		typedef std::deque< const ServerConf * >::iterator iterator;
		typedef std::deque< const ServerConf * >::const_iterator const_iterator;
		
		static int add ( std::ifstream & );
		static void clear ( void );
		
		friend std::ostream & operator << ( std::ostream&, const ServerConf & );

	private:
		
		//  An empty ServerConf instance 
		//  may be created for testing purposes.
		
		ServerConf ( void );
		ServerConf ( const ServerConf & );
		ServerConf ( const std::deque< std::string > & );
		ServerConf & operator = ( const ServerConf & );
		~ServerConf ( void );

		static int file2mem ( std::ifstream &, std::deque< std::string > & );
		static void normalize ( std::deque< std::string > & );
		static void split_elements ( std::deque< std::string > & );
		static void split2blocks( std::deque< std::string > &,
				std::deque< std::deque< std::string > > &, const char * );
		static int pre_parse ( std::deque< std::string > & );
		static int parse ( std::deque< std::string > &,
				std::deque< std::deque< std::string > > & );
		
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
		unsigned int 				_allow_methods;
		std::size_t					_client_max_body_size;
		std::vector< std::string>	_index;
		std::string 				_cgi_param; //nse si guardar en un vector
		std::string					_cgi_pass; 
		std::map<int, std::string>	_error_page; //falta fer
		
};
