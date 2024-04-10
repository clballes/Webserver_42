/*                                                                            */
/* delete.cpp                                                                 */
/* mpuig-ma <mpuig-ma@student.42barcelona.com>                                */
/* Tue Mar 19 12:48:11 2024                                                   */

#include "HTTP.hpp"
#include "parse.hpp"

int
HTTP::http_delete ( HTTP & http )
{
	LOG( "call HTTP::http_delete()" );
	std::string target;
	target.append( http._server._root );
	target.append( http._request.target );

	LOG( " target: " << target );

	if (is_regular_file(target))
	{
		if (remove(target.c_str()) == 0)
		{
			//http._status_code = 204; //no content per tant no message body rq
			http._status_code = 200; //si que retorna missatge
			http._message_body.append("<!DOCTYPE html><body><h1>File deleted.</h1></body></html>");
		}
			
		else
		{
			http._status_code = 403;
			HTTP::load_file( http, http._server._error_page[403] );
		}
	}
	else
	{
		http._status_code = 404;
		HTTP::load_file( http, http._server._error_page[404] );
	}

	return ( EXIT_SUCCESS );
}
