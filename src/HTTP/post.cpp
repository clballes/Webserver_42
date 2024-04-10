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
HTTP::http_post ( HTTP & http ){

	LOG( "call HTTP::http_post()" );

	if(	http._status_code == INTERNAL_SERVER_ERROR)
	{
		LOG( http._server._error_page[500] );
		(void) HTTP::load_file( http, http._server._error_page[500] );
	}
	if ( http._server._cgi_pass.length() != 0 )
	{
		LOG( " Executing Cgi" );
		CGI *cgi_ptr = new CGI( http );
		if (cgi_ptr->execute() == EXIT_FAILURE)
			return EXIT_FAILURE;
		std::cout << " ---------------------------------------------------------------------------------------------------------------- AFTER CGI AAAAAA A A A A"<< http._message_body << std::endl;
		http._status_code = 200;
	}
	else
	{
		http._status_code = 204;
		http._message_body = "";
	}
	return ( EXIT_SUCCESS );
}