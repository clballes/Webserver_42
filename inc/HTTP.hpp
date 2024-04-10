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
#include "IEvent.hpp"
#include "Server.hpp"
#include "define.hpp"
#include "debug.hpp"
#include "CGI.hpp"


typedef std::map< std::string, std::string > t_headers;

class HTTP;
class CGI;

typedef struct s_http_method
{
	const char * method;
	int ( *method_func )( HTTP & );
	int code;

} t_http_method;

typedef struct s_request
{
	t_http_method * method;
	std::string target;
	std::string query;
	std::string body;
	std::string fragments;
	int http_version;

} t_request;

class Server;

class HTTP: public IEvent
{
	public:

		HTTP ( Server & );
		~HTTP ( void );

		void dispatch ( struct kevent & );
		int register_recv ( void );
		int register_send ( void );
		int request_recv ( int64_t );
		int request_send ( void );

		static std::string & urlencode ( std::string & );
		static std::string & urldecode ( std::string & );
		static int load_file ( HTTP &, std::string );
		int put_file( void );
		void generateHTML(); // aico sera per el CGI

		void perform ( void );

		static int n_methods;
		static std::size_t n_longest_method;
		static t_http_method methods[];

		std::string getTarget( void );
		std::string getCGIpass( void );

	private:

		int						_socket_fd;
		unsigned				_address_len;
		struct sockaddr_in		_address;
		Server &                _server;
		CGI *cgi_ptr;			//LIBERAR MEMORIA

		t_headers _request_headers;
		t_headers _response_headers;
		std::string _buffer_recv;
		std::string _buffer_send;
		std::string _message_body;
		t_request _request;
		int _status_code;
		bool _keep_alive; //?

		int parse ( void );
		int parse_start_line ( std::string & );
		int parse_field_line ( std::string & );
		static int http_get ( HTTP & );
		static int http_head ( HTTP & );
		static int http_post ( HTTP & );
		static int http_put ( HTTP & );
		static int http_delete ( HTTP & );
		static int compose_response ( HTTP & );
		static int autoindex ( HTTP & );

};

#endif /* !_HTTP_HPP_ */