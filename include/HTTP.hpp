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

class HTTP;
class CGI;
class Router;
class Server;
class Connection;

typedef std::map< std::string, std::string > t_headers;

typedef struct s_http_method
{
	const char *	method;
	int				( *method_func )( HTTP & );
	int				code;

} t_http_method;

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

typedef struct s_response
{
	std::string		body;
	t_headers		headers;

} t_response;

class HTTP: public IEvent
{
	public:

		HTTP ( Router &, int );
		~HTTP ( void );

		void dispatch ( struct kevent & );
		int register_recv ( void );
		int register_send ( void );
		int deregister_recv ( void );
		int recv_request ( int64_t );
		int send_response ( void );
		int compose_response ( void );
		int compute_response ( void );
		int check_index ( void ); // can be made in-file static

		Server & getServer ( void );
		t_request & getRequest ( void );
		t_headers & getRequestHeaders ( void );
		t_response & getResponse ( void );
		t_headers & getResponseHeaders ( void );
		
		void setMessageBody( const std::string & );
		void setStatusCode( int );
		void setResponseHeaders( const std::string &, const std::string & );
		
		static t_http_method methods[];
		static int handle_chunk_expect ( HTTP & ); // can be made in-file static

	private:

		int						_socket_fd;
		unsigned				_address_len;
		struct sockaddr_in		_address;
		Router &				_router;
		Connection &			_connection;
		Server &                _server;
		CGI *					_cgi_ptr;

		int						_status_code;
		std::string				_buffer_recv;
		std::string				_buffer_send;
		t_request				_request;
		t_response				_response;
		bool					_keep_alive;
		bool					_expect;

		int parse ( void );
		int parse_start_line ( std::string & ); // can be made in-file static
		int parse_field_line ( std::string & ); // can be made in-file static
		
		static int http_get ( HTTP & );
		static int http_head ( HTTP & );
		static int http_post ( HTTP & );
		static int http_put ( HTTP & );
		static int http_delete ( HTTP & );
		static int autoindex ( HTTP & );
};

#endif /* !_HTTP_HPP_ */
