/*                                                                            */
/* put.cpp                                                                    */
/* mpuig-ma <mpuig-ma@student.42barcelona.com>                                */
/* Tue Mar 19 12:48:28 2024                                                   */

#include "HTTP.hpp"
#include "parse.hpp"

int HTTP::put_file()
{
	std::string target;

	target.append( this->_server._root );
	target.append( this->_request.target );
	std::ofstream	file;

	if (is_regular_file(target))
	{
		file.open(target.c_str());
		file << this->_request.body;
		file.close();
		return (204);
	}
	else
	{
		file.open(target.c_str(), std::ofstream::out | std::ofstream::trunc);
		if (file.is_open() == false)
			return (403);
		file << this->_request.body;
		file.close();
		return (201);
	}
}

int
HTTP::http_put ( HTTP & http )
{
	LOG( "call HTTP::http_put()" );

	std::string target;

	target.append( http._server._root );
	target.append( http._request.target );

	LOG( " target: " << target );

	std::cout << "content body: "<<  http._request.body << std::endl;
	http._status_code = http.put_file(  );
	std::cout << "code is: "<< http._status_code << std::endl;
	// if(http._status_code == 201) // todo ok, 201 created
	//(void) HTTP::load_file( http, http._server._error_page[404] );
	// else if(http._status_code == 204) //no content, tot ok
	//if(http._status_code == 200) podria ferla es tot ok

	// _code = writeContent(content);
	// if (_code != 201 && _code != 204)
	// 	_response = this->readHtml(_errorMap[_code]);
	// _response = head.getHeader(_response.size(), target, _code, _type, requestConf.getContentLocation(), requestConf.getLang()) + "\r\n" + _response;
	return ( EXIT_SUCCESS );
}
