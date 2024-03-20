/*                                                                            */
/* destructor.cpp                                                             */
/* mpuig-ma <mpuig-ma@student.42barcelona.com>                                */
/* Wed Mar 20 12:29:28 2024                                                   */

#include "CGI.hpp"

CGI::~CGI ( void )
{
	LOG( "call CGI::~CGI()" );

	if ( this->_env == 0x0 )
		return ;

	for ( std::size_t i = 0; this->_env[i] != 0x0; ++i )
		delete ( this->_env[i] );

	delete ( this->_env );

	return ;
}
