/*                                                                            */
/* HTTP.hpp                                                                   */
/* mpuig-ma <mpuig-ma@student.42barcelona.com>                                */
/* Tue Apr  9 16:50:33 2024                                                   */

#ifndef _HTTP_HPP_
#define _HTTP_HPP_

#include <string>
#include <iostream>
#include <fstream>
#include <map>
#include <unistd.h> 		/* close */
#include <cstdlib>
#include <cstring>
#include <sys/stat.h>

#include "IEvent.hpp"
#include "Router.hpp"
#include "Server.hpp"
#include "Connection.hpp"
#include "CGI.hpp"
#include "define.hpp"
#include "log.hpp"
#include "string.hpp"
#include "file.hpp"
#include "HTTP_status_codes.hpp"

typedef std::map< std::string, std::string > t_headers;

class HTTP;
class CGI;
class Router;
class Server;
class Connection;

typedef struct s_http_method
{
	const char * method;
	int ( *method_func )( HTTP & );
	int code;
} t_http_method;

typedef struct s_request
{
	t_http_method * method;
	std::string host;
	std::string target;
	std::string file;
	struct stat file_info;
	std::string query;
	std::string body;
	int http_version;

} t_request;

class HTTP: public IEvent
{
	public:

		HTTP ( Router &, int );
		~HTTP ( void );

		void dispatch ( struct kevent & );
		int register_recv ( void );
		int register_send ( void );
		int request_recv ( int64_t );
		int request_send ( void );
		void perform ( void );

		static int load_file ( HTTP &, std::string );
		// int put_file( void );
		int check_index();

		static t_http_method methods[];

		t_request & getRequest( void );
		t_headers & getHeaders( void );
		Server & getServer();
		std::string & getCGIpass( void );
		void set_message_body( std::string & );
		void setStatusCode( int );
		void set_response_headers( std::string arg, std::string value );

	private:

		int						_socket_fd;
		unsigned				_address_len;
		struct sockaddr_in		_address;
		Router &				_router;
		Connection &			_connection;
		Server &                _server;
		CGI *cgi_ptr;			//LIBERAR MEMORIA

		t_headers				_request_headers;
		t_headers				_response_headers;
		std::string				_buffer_recv;
		std::string				_buffer_send;
		std::string				_message_body;
		t_request				_request;
		int						_status_code;
		std::string				_redirection_str;
		bool					_keep_alive; //?

		int parse ( void );
		int parse_start_line ( std::string & );
		int parse_field_line ( std::string & );
		static int http_get ( HTTP & );
		static int http_head ( HTTP & );
		static int http_post ( HTTP & );
		// static int http_put ( HTTP & );
		static int http_delete ( HTTP & );
		static int compose_response ( HTTP & );
		static int autoindex ( HTTP & );
};

#endif /* !_HTTP_HPP_ */
