/*                                                                            */
/* HTTP.hpp                                                                   */
/* mpuig-ma <mpuig-ma@student.42barcelona.com>                                */
/* Wed Feb 28 17:46:13 2024                                                   */

#ifndef _HTTP_HPP_
#define _HTTP_HPP_

#include "IEvent.hpp"
#include "Client.hpp"
#include <string>
#include <iostream>

#define BAD_REQUEST			400
#define METHOD_NOT_ALLOWED	405
#define URI_TOO_LONG		414
#define NOT_IMPLEMENTED		501

#define HTTP_GET			0x00000001
#define HTTP_HEAD			0x00000002
#define HTTP_POST			0x00000003
#define HTTP_PUT			0x00000004
#define HTTP_DELETE			0x00000005

class Client;
class HTTP;

typedef struct s_http_method
{
	const char * method;
	int ( *method_func )( HTTP & );

} t_http_method;

typedef struct s_request
{
	int method;
	char * request_target;
	int http_version;

} t_request;

class HTTP: public IEvent
{
	public:

		HTTP ( const Client & );
		~HTTP ( void );

		static int            n_methods;
		static t_http_method  methods[];

		static int http_get ( HTTP & );
		static int http_head ( HTTP & );
		static int http_post ( HTTP & );
		static int http_put ( HTTP & );
		static int http_delete ( HTTP & );

		void dispatch ( struct kevent & );
		void perform ( void );

	private:

		const Client & _client;

		t_request      _request_line;
		int	 		   _response_code;

		bool           _keep_alive;
		std::string    _host;
		std::string    _user_agent;

		int start_line ( void );
		void parse ( void );

};

#endif /* !_HTTP_HPP_ */
