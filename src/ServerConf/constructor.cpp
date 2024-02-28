/*                                                                            */
/* constructor.cpp                                                            */
/* mpuig-ma <mpuig-ma@student.42barcelona.com>                                */
/* Thu Feb 15 16:49:35 2024                                                   */

#include "Log.hpp"
#include "ServerConf.hpp"
#include <deque>

// We use <deque> because we don't know beforehand
// how many `server {}' there are.

std::deque< const ServerConf * >
ServerConf::instances;

t_configuration_directives
ServerConf::_config_directives[] = {
	{ "listen", &ServerConf::set_listen },
	{ "root", &ServerConf::set_root },
	{ "server_name", &ServerConf::set_server_name },
	{ "error_page", &ServerConf::set_error_page },
	{ "client_body", &ServerConf::set_client_body },
	{ "cgi_param", &ServerConf::set_cgi_param },
	{ "cgi_pass", &ServerConf::set_cgi_pass },
	{ "allow_methods", &ServerConf::set_allow_methods, },
	{ "index", &ServerConf::set_index },
	{ "autoindex", &ServerConf::set_autoindex },
	{ 0x0, 0x0 } };

ServerConf::ServerConf ( void ): good( true )
{
	LOG( "call ServerConf::ServerConf( void )" );

	return ;
}

ServerConf::ServerConf ( const ServerConf & instance )
{
	*this = instance;
	
	LOG( "call ServerConf::ServerConf( const ServerConf & )" );
	
	return ;
}

ServerConf::ServerConf ( const std::deque< std::string > & server_block )
{
	this->good = true;

	LOG( "call ServerConf::ServerConf( const std::deque< std::string > &" );
	
	if ( ServerConf::set_directives( server_block ) == EXIT_FAILURE )
	{
		std::cerr << "error: set_directives" << std::endl;
		this->good = ! good;
	}

	return ;
}

ServerConf &
ServerConf::operator = ( const ServerConf & instance )
{
	(void) instance;

	this->good = instance.good;

	LOG( "call ServerConf::operator=ServerConf" );

	return (*this);
}

std::ostream &
operator << ( std::ostream & out, const ServerConf & instance )
{
	std::vector< std::string >::const_iterator it;

	out << "listen: " << inet_ntoa( instance._address.sin_addr );              // @mpuig-ma
																			   // Function not allowed
	out << " " << ntohs( instance._address.sin_port );

	out << std::endl;

	out << "  server_name:";
	for ( it = instance._server_name.begin();
			it != instance._server_name.end(); ++it )
		out << " " << *it;
	out << std::endl;

	out << "  root: " << instance._root;
	out << std::endl;

	out << "  allow_methods:";
	for ( it = instance._allow_methods.begin();
			it != instance._allow_methods.end(); ++it )
		out << " " << *it;
	out << std::endl;

	out << "  client_max_body_size: " << instance._client_max_body_size;
	out << std::endl;

	out << "  cgi_pass: " << instance._cgi_pass;
	out << std::endl;

	out << "  cgi_param: " << instance._cgi_param;
	out << std::endl;

	out << "  index: " << instance._index;
	out << std::endl;
	
	out << "  autoindex: " << std::boolalpha << instance._autoindex;
	//out << std::endl;

	return ( out );
}
