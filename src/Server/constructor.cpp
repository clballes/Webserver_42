/*                                                                            */
/* constructor.cpp                                                            */
/* mpuig-ma <mpuig-ma@student.42barcelona.com>                                */
/* Wed Feb 21 15:49:35 2024                                                   */

#include "Server.hpp"

// Initialize static `vector' of servers

std::vector< Server * >
Server::servers;

Server::Server ( const ServerConf & instance ): good( true )
{
	LOG( "call Server::Server( const ServerConf & )" );
	
	// This should only be logged on certain mode
	//LOG( instance );

	std::memcpy( &this->_address, &instance._address,
			sizeof( instance._address ) );

	_server_name = instance._server_name;
	_root = instance._root;
	_allow_methods = instance._allow_methods;
	_client_max_body_size = instance._client_max_body_size;
	_index = instance._index;
	_cgi_param = instance._cgi_param;
	_cgi_pass = instance._cgi_pass;
	_error_page = instance._error_page;

	return ;
}
