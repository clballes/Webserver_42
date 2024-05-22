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
#include <unistd.h>
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

#include "t_headers.hpp"
#include "t_request.hpp"
#include "t_response.hpp"
#include "t_http_method.hpp"

#define PENDING_START_LINE	0
#define PENDING_HEADERS		1
#define PENDING_BODY		2
#define COMPLETE			3

class HTTP;
class CGI;
class Router;
class Server;
class Connection;

class HTTP: public IEvent
{
	public:

		HTTP ( Router &, int );
		~HTTP ( void );

		void dispatch ( struct kevent & );
		int register_recv ( void );
		int register_send ( void );
		int register_timer ( void );
		int deregister_recv ( void );
		int deregister_timer ( void );
		int recv_request ( int64_t );
		int send_response ( void );
		int compose_response ( void );
		int compute_response ( void );

		Server & getServer ( void );
		t_request & getRequest ( void );
		t_response & getResponse ( void );

		void setMessageBody( const std::string & );
		void setStatusCode( int );
		void setState( int );
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

		int						_state;
		bool					_keep_alive;
		bool					_expect;
		std::string				_buffer_recv;
		std::string				_buffer_send;
		t_request				_request;
		t_response				_response;

		int parse ( void );

		static int http_get ( void * HTTP );
		static int http_head ( void * HTTP );
		static int http_post ( void * HTTP );
		static int http_put ( void * HTTP );
		static int http_delete ( void * HTTP );
};

#endif /* !_HTTP_HPP_ */
