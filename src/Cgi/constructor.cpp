/*                                                                            */
/* constructor.cpp                                                            */
/* mpuig-ma <mpuig-ma@student.42barcelona.com>                                */
/* Wed Mar 20 12:29:32 2024                                                   */

#include "CGI.hpp"
#include "ft_stdlib.h"

CGI::CGI ( HTTP & http ): _http( http ), _env( 0x0 )
{
	LOG( "call CGI::CGI( HTTP & )" );

	this->_env = new char * [4];
	this->_env[0] = ft_strdup( "REQUEST_METHOD=GET" );
	this->_env[1] = ft_strdup( "PATH_INFO=" );
	this->_env[2] = ft_strdup( "SERVER_PROTOCOL=HTTP/1.1" );
	this->_env[3] = nullptr;

	(void) http;
	// aqui afegim coses 
	

	return ;
}
