/*                                                                            */
/* HTTP.cpp                                                                   */
/* mpuig-ma <mpuig-ma@student.42barcelona.com>                                */
/* Tue Apr  9 16:50:09 2024                                                   */

#include "HTTP.hpp"

int
HTTP::http_head ( HTTP & http )
{
	DEBUG( "" );
	if (http._server.getCGIpass().length() != 0 )
	{}
	else
	{}
	(void) http;
	return ( EXIT_SUCCESS );
}

int
HTTP::http_get ( HTTP & http )
{
	DEBUG( http._request.target );
	// If file is not accessible
	// and autoindex is on try accessing dir.
	if ( http._status_code >= 300 )
	{
		LOG( http._server.getErrorPage( http._status_code) );
		(void) HTTP::load_file( http, http._server.getErrorPage( http._status_code ) );
		return ( EXIT_FAILURE );
	}
	if ( is_regular_file( http._request.target ) == true )
	{
		http._status_code = HTTP::load_file( http, http._request.target );
	}
	else if ( http._server.getFlags() & F_AUTOINDEX ) // las server._flags crec q es aixo pero nose 
	{
		http._status_code = HTTP::autoindex( http );
	}
	else
		http._status_code = INTERNAL_SERVER_ERROR;

	if ( http._server.getCGIpass().length() != 0 )
	{
		http.cgi_ptr = new CGI( http );
		if ( http.cgi_ptr->execute() == EXIT_FAILURE )
			return ( EXIT_FAILURE );
		http._status_code = 200;
	}
	else
	{
		// no content status code
	}	
	// implement loop fashion
	return ( EXIT_SUCCESS );
}

void HTTP::generateHTML()
{
    std::string page;
    page.append( "<!DOCTYPE html><html lang='en'><meta charset='UTF-8'><meta name='viewport' content='width=device-width, initial-scale=1.0'>" );
    page.append( "<title>Post Response</title>" );
    // Add CSS styling
    page.append( "<style>" );
    page.append( "body { font-family: Arial, sans-serif; background-color: #f4f4f4; }" );
    page.append( ".container { max-width: 800px; margin: 20px auto; padding: 20px; background-color: #fff; border-radius: 10px; box-shadow: 0 2px 5px rgba(0,0,0,0.1); }" );
    page.append( "h1 { color: #333; text-align: center; }" );
    page.append( "h2 { color: #555; text-align: center; }" );
    page.append( "p { margin: 10px 0; text-align: center; }" );
    page.append( "</style>" );
    page.append( "</head>" );
    page.append( "<body>" );
    // Add a container div for better styling
    page.append( "<div class='container'>" );
    page.append( "<h1> POST RUNNING SUCCESSFULLY </h1>" );
    page.append( "<h2> CONGRATULATIONS, YOUR DATA HAS BEEN SAVED :) </h2>" );
    // Parsing the HTTP request body to generate response
    size_t startPos = 0;
    size_t endPos = 0;
	std::string decoded_request = urldecode( this->_request.body );
	while ( ( endPos = decoded_request.find( '&', startPos ) ) != std::string::npos )
	{
		std::string substr = decoded_request.substr( startPos, endPos - startPos );
		// Process substring: replace = with : 
		size_t equalPos = substr.find( '=' );
		if ( equalPos != std::string::npos )
		{
			substr.replace( equalPos, 1, ": " );
		}
		page.append( "<p>" );
		page.append( substr );
		page.append( "</p>" );
		// Move startPos to next position after '&'
		startPos = endPos + 1;
	}
    // Process the last substring (after the last '&')
    std::string lastSubstring = decoded_request.substr( startPos );
    size_t equalPos = lastSubstring.find( '=' );
    if ( equalPos != std::string::npos )
	{
        lastSubstring.replace( equalPos, 1, ": " );
    }
	page.append( "<p>" );
	page.append( lastSubstring );
	page.append( "</p>" );
    page.append( "<p>" );
    page.append( "</p>" );
    // Close container div
    page.append( "</div>" );
    page.append( "</body>" );
    page.append( "</html>" );
    this->_message_body.append( page.c_str() );
}

int
HTTP::http_post ( HTTP & http )
{
	DEBUG( "" );
	if ( http._status_code == INTERNAL_SERVER_ERROR )
	{
		LOG( http._server.getErrorPage(500) );
		(void) HTTP::load_file( http, http._server.getErrorPage(500) );
	}
	if ( http._server.getCGIpass().length() != 0 )
	{
		LOG( " Executing Cgi" );
		CGI *cgi_ptr = new CGI( http );
		if ( cgi_ptr->execute() == EXIT_FAILURE )
			return ( EXIT_FAILURE );
		http._status_code = 200;
	}
	else
	{
		http._status_code = 204;
		http._message_body.clear();
	}
	return ( EXIT_SUCCESS );
}

// TODO: pendent removal
int HTTP::put_file()
{
	std::ofstream file;

	if ( is_regular_file( this->_request.target ) )
	{
		file.open( this->_request.target.c_str() );
		file << this->_request.body;
		file.close();
		return ( 204 );
	}
	file.open( this->_request.target.c_str(), std::ofstream::out | std::ofstream::trunc );
	if ( file.is_open() == false )
		return ( 403 );
	file << this->_request.body;
	file.close();
	return ( 201 );
}

int
HTTP::http_put ( HTTP & http )
{
	DEBUG( "" );
	http._status_code = http.put_file();
	// if(http._status_code == 201) // todo ok, 201 created
	//(void) HTTP::load_file( http, http._server._conf._error_page[404] );
	// else if(http._status_code == 204) //no content, tot ok
	//if(http._status_code == 200) podria ferla es tot ok
	// _code = writeContent(content);
	// if (_code != 201 && _code != 204)
	// 	_response = this->readHtml(_errorMap[_code]);
	// _response = head.getHeader(_response.size(), target, _code, _type, requestConf.getContentLocation(), requestConf.getLang()) + "\r\n" + _response;
	return ( EXIT_SUCCESS );
}

int
HTTP::http_delete ( HTTP & http )
{
	DEBUG( "" );
	if ( is_regular_file( http._request.target ) )
	{
		if ( remove( http._request.target.c_str() ) == 0 )
		{
			http._status_code = 200;
			http._message_body.append( "<!DOCTYPE html><body><h1>File deleted.</h1></body></html>" );
		}
		else
		{
			http._status_code = 403;
			HTTP::load_file( http, http._server.getErrorPage( 403 ) );
		}
	}
	else
	{
		http._status_code = 404;
		HTTP::load_file( http, http._server.getErrorPage( 404 ) );
	}
	return ( EXIT_SUCCESS );
}
