/*                                                                            */
/* constructor.cpp                                                            */
/* mpuig-ma <mpuig-ma@student.42barcelona.com>                                */
/* Wed Feb 28 16:18:46 2024                                                   */

#include "Log.hpp"
#include "HTTP.hpp"
#include <unistd.h>

t_http_method
HTTP::methods[] = {
	{ "GET", &HTTP::http_get },
	{ "HEAD", &HTTP::http_head },
	{ "POST", &HTTP::http_post },
	{ "PUT", &HTTP::http_put },
	{ "DELETE", &HTTP::http_delete },
	{ 0, 0 }
};

int
HTTP::n_methods = ( sizeof( HTTP::methods ) /
		sizeof( *HTTP::methods ) ) - 1;

HTTP::HTTP ( const Client & client_instance ): _client( client_instance )
{
	LOG( "call HTTP::HTTP( const Client & )" );
	
	(void) _host;
	(void) _user_agent;

	return ;
}
