/*                                                                            */
/* constructor.cpp                                                            */
/* mpuig-ma <mpuig-ma@student.42barcelona.com>                                */
/* Wed Feb 28 16:18:46 2024                                                   */

#include "Log.hpp"
#include "HTTP.hpp"
#include <unistd.h>
#include <ft_string.h>

t_http_method
HTTP::methods[] = {
	{ "GET", &HTTP::http_get, HTTP_GET },
	{ "HEAD", &HTTP::http_head, HTTP_HEAD },
	{ "POST", &HTTP::http_post, HTTP_POST },
	{ "PUT", &HTTP::http_put, HTTP_PUT },
	{ "DELETE", &HTTP::http_delete, HTTP_DELETE },
	{ 0, 0, 0 }
};

static int get_method_longest_len ( t_http_method * );

int
HTTP::n_methods = ( sizeof( HTTP::methods ) /
		sizeof( *HTTP::methods ) ) - 1;

std::size_t
HTTP::n_longest_method = get_method_longest_len( &HTTP::methods[0] );

HTTP::HTTP ( Client & client_instance, Server & server_instance ):
	_client( client_instance ),
	_server( server_instance )
{
	LOG( "call HTTP::HTTP( Client & )" );

	std::memset( &this->_request, 0x0, sizeof( this->_request ) );

	this->_request.target = this->_server._root;
	
	return ;
}

static int
get_method_longest_len ( t_http_method * ptr )
{
	size_t n;

	n = 0;
	while ( ptr->method != NULL )
	{
		if ( ft_strlen( ptr->method ) > n )
			n = ft_strlen( ptr->method );
		++ptr;
	}

	return ( n );
}
