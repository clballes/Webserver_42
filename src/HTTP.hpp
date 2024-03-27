/*                                                                            */
/* HTTP.hpp                                                                   */
/* mpuig-ma <mpuig-ma@student.42barcelona.com>                                */
/* Wed Feb 28 17:46:13 2024                                                   */

#ifndef _HTTP_HPP_
#define _HTTP_HPP_

#include <string>
#include <iostream>
#include "Log.hpp"
#include "HTTP_status_codes.hpp"
#include "CGI.hpp"
#include <map>

#define HTTP_GET		0x00000001
#define HTTP_HEAD		0x00000002
#define HTTP_POST		0x00000003
#define HTTP_PUT		0x00000004
#define HTTP_DELETE		0x00000005

#define HTTP_11			0x11

#define LF 012
#define CR 015
#define SP 040

class HTTP;
class Client;
class Server;
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

typedef std::map< std::string, std::string > t_headers;

class HTTP
{
	public:

		HTTP ( Client &, Server & );
		~HTTP ( void );
		
		void perform ( void );

		static int            n_methods;
		static std::size_t    n_longest_method;
		static t_http_method  methods[];
		
		static std::string & urlencode ( std::string & );
		static std::string & urldecode ( std::string & );
		static int load_file ( HTTP &, std::string & );

		friend class CGI;
	protected:

		Client &                _client;
		Server &			    _server;
		t_headers               _headers;
		t_headers               _response_headers;

		std::string			    _buffer_recv;
		std::string   			_buffer_send;
		std::string				_message_body;
		
		t_request            	_request;
		int                   	_status_code;
		bool                  	_keep_alive;
				
		int parse ( void );
		int parse_start_line ( std::string & );
		int parse_field_line ( std::string & );
		void generateHTML();
		int put_file();
		
		static int http_get ( HTTP & );
		static int http_head ( HTTP & );
		static int http_post ( HTTP & );
		static int http_put ( HTTP & );
		static int http_delete ( HTTP & );
		static int compose_response ( HTTP & );
		static int autoindex ( HTTP &, std::string & );

};

#include "Client.hpp"
//#include "Server.hpp"

#endif /* !_HTTP_HPP_ */
