/*                                                                            */
/* post.cpp                                                                   */
/* mpuig-ma <mpuig-ma@student.42barcelona.com>                                */
/* Tue Mar 19 12:47:34 2024                                                   */

#include "HTTP.hpp"

int
HTTP::http_post ( HTTP & http )
{
	std::string target;

	LOG( "call HTTP::http_post()" );

	target.append( http._server._root );
	target.append( http._request.target );

	LOG( " target: " << target );

	if ( http._server._cgi_pass.length() != 0 )
	{
		std::cout <<" ------------------- "<< std::endl;
		LOG( " Executing Cgi for post method " );
		http.execute();
		std::cout <<" ------------------- "<< std::endl;

	}

	if (!http._request.body.empty()) //SI EXISTEIX INFO QUE MOSTRAR EN LA RESPONSE DEL POST 
	{
		http._status_code = 200;
		std::string page;
		page.append( "<!DOCTYPE html>" );
		page.append( "<head>" );
		page.append( "<title>" );
		page.append( "Post Response" );
		page.append( "</title>" );
		page.append( "<body>" );
		
		//parsing the http reuest bdy to give a post respoonse:
		std::cout << "responseeeeee: " <<  http._request.body<< std::endl;
		std::size_t start_pos = 0;
		std::size_t end_pos;

		// Find substrings until '&' is found
		while ((end_pos = http._request.body.find('&', start_pos)) != std::string::npos) {
			std::string substring = http._request.body.substr(start_pos, end_pos - start_pos);
			page.append(substring);
			// Update start position for next substring
			start_pos = end_pos + 1;
		}
    std::string last_substring = http._request.body.substr(start_pos);
  
		page.append(last_substring);

		
		page.append( "POST RUNNING SUCESFULLY!! CONGRATULATIONS YOUR DATA HAS BEEN SAVED :)" );
		page.append( "</body>" );
		page.append( "</head>" );
		page.append( "</html>" );


		http._message_body.append( page.c_str() );
		// std::cout << << 
	}
	else
		http._status_code = INTERNAL_SERVER_ERROR;
		// _https_status.code = 204; // ggeenretare response without content length 

	return ( EXIT_SUCCESS );
}