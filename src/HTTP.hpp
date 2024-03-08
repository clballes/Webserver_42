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

#define HTTP_GET				0x00000001
#define HTTP_HEAD				0x00000002
#define HTTP_POST				0x00000003
#define HTTP_PUT				0x00000004
#define HTTP_DELETE				0x00000005

#define HTTP_11					0x11

#define LF 012
#define CR 015
#define SP 040

class HTTP;

typedef struct s_http_method
{
	const char * method;
	int ( *method_func )( HTTP & );
	int code;

} t_http_method;

typedef struct s_request
{
	int method;
	char * request_target;
	int http_version;

} t_request;

class HTTP
{
	public:

		HTTP ( void );
		~HTTP ( void );

		static int            n_methods;
		static int            n_longest_method;
		static t_http_method  methods[];

		static int http_get ( HTTP & );
		static int http_head ( HTTP & );
		static int http_post ( HTTP & );
		static int http_put ( HTTP & );
		static int http_delete ( HTTP & );

		void perform ( void );

	protected:

		int64_t					_data_recv;
		std::string			    _buffer_recv;
		std::string   			_buffer_send;
		
		t_request            	_request_line;
		int                   	_status_code;
		bool                  	_keep_alive;

		int parse ( void );
		int parse_start_line ( void );
		int parse_method ( char *, int64_t * );
		int parse_request_target ( char *, int64_t * );
		int parse_http_version ( char *, int64_t * );
		int parse_field_lines ( void );
};

#endif /* !_HTTP_HPP_ */
