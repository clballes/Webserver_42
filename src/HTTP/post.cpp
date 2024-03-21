/*                                                                            */
/* post.cpp                                                                   */
/* mpuig-ma <mpuig-ma@student.42barcelona.com>                                */
/* Tue Mar 19 12:47:34 2024                                                   */

#include "HTTP.hpp"

void HTTP::generateHTML() {
    std::string page;
    page.append("<!DOCTYPE html><html lang='en'><meta charset='UTF-8'><meta name='viewport' content='width=device-width, initial-scale=1.0'>");
    page.append("<title>Post Response</title>");
    // Add CSS styling
    page.append("<style>");
    page.append("body { font-family: Arial, sans-serif; background-color: #f4f4f4; }");
    page.append(".container { max-width: 800px; margin: 20px auto; padding: 20px; background-color: #fff; border-radius: 10px; box-shadow: 0 2px 5px rgba(0,0,0,0.1); }");
    page.append("h1 { color: #333; text-align: center; }");
    page.append("h2 { color: #555; text-align: center; }");
    page.append("p { margin: 10px 0; text-align: center; }");
    page.append("</style>");
    page.append("</head>");
    page.append("<body>");
    // Add a container div for better styling
    page.append("<div class='container'>");
    page.append("<h1> POST RUNNING SUCCESSFULLY </h1>");
    page.append("<h2> CONGRATULATIONS, YOUR DATA HAS BEEN SAVED :) </h2>");
    // Parsing the HTTP request body to generate response
    size_t startPos = 0;
    size_t endPos = 0;
	std::string decoded_request = urldecode(_request.body);
	while ((endPos = decoded_request.find('&', startPos)) != std::string::npos) {
		std::string substr = decoded_request.substr(startPos, endPos - startPos);
		
		// Process substring: replace = with : 
		size_t equalPos = substr.find('=');
		if (equalPos != std::string::npos) {
			substr.replace(equalPos, 1, ": ");
		}
	page.append( "<p>" );
	page.append( substr );
	page.append( "</p>" );
	// Move startPos to next position after '&'
	startPos = endPos + 1;
	}

    // Process the last substring (after the last '&')
    std::string lastSubstring = decoded_request.substr(startPos);
    size_t equalPos = lastSubstring.find('=');
    if (equalPos != std::string::npos) {
        lastSubstring.replace(equalPos, 1, ": ");
    }
	page.append( "<p>" );
	page.append( lastSubstring );
	page.append( "</p>" );
    page.append("<p>");
    page.append("</p>");
    // Close container div
    page.append("</div>");
    page.append("</body>");
    page.append("</html>");
    _message_body.append(page.c_str());
}


int
HTTP::http_post ( HTTP & http )
{
	std::string target;

	LOG( "call HTTP::http_post()" );

	target.append( http._server._root );
	target.append( http._request.target );

	LOG( " target: " << target );

	if ( http._server._cgi_pass.length() != 0 ) //nose si va junt o per separat, entenc que si te cgi ho fa atraves del cgi pero poder el post despres tamb el pugui fer sense cgi DUDA
	{
		std::cout <<" ------------------- "<< std::endl;
		LOG( " Executing Cgi for post method " );
		http.execute();
		std::cout <<" ------------------- "<< std::endl;
		http.generateHTML(); 
   		http._status_code = 200;
	}
	// else if (!http._request.body.empty()) //SI EXISTEIX INFO QUE MOSTRAR EN LA RESPONSE DEL POST 
	// {
   	// 	http.generateHTML(); 
   	// 	http._status_code = 200;
	// }
	else
	{
		// _https_status.code = 204; 
		// loadfile ---- ggeenretare response without content length 
	}
	// if(	http._status_code = INTERNAL_SERVER_ERROR)
	//algo aixi q peti si ha eptat per algun lloc

	return ( EXIT_SUCCESS );
}