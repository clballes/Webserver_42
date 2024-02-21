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

ServerConf::ServerConf ( void )
{
	LOG( "call ServerConf( void )" )

	return ;
}

ServerConf::ServerConf ( const ServerConf & instance )
{
	*this = instance;
	
	LOG( "call ServerConf( const ServerConf & )" )
	
	return ;
}

ServerConf::ServerConf ( const std::deque< std::string > & server_block )
{
	LOG( "call ServerConf( const std::deque< std::string > &" )
	
	if ( ServerConf::set_directives( server_block ) == EXIT_FAILURE )
	{
		std::cerr << "ServerConf: error: check_directives" << std::endl;
	}

	return ;
}

ServerConf &
ServerConf::operator = ( const ServerConf & instance )
{
	(void) instance;

	LOG( "call operator=ServerConf" )

	return (*this);
}

std::ostream &
operator << ( std::ostream & out, const ServerConf & instance )
{
	std::vector< std::string >::const_iterator it;

	out << "listen: " << ntohs( instance._address.sin_port );
	out	<< " " << ntohl( instance._address.sin_addr.s_addr );
	out << std::endl;

	out << "server_name:";
	for ( it = instance._server_name.begin();
			it != instance._server_name.end(); ++it )
		out << " " << *it;
	out << std::endl;

	out << "root: " << instance._root;
	out << std::endl;

	out << "allow_methods:";
	for ( it = instance._allow_methods.begin();
			it != instance._allow_methods.end(); ++it )
		out << " " << *it;
	out << std::endl;

	out << "client_max_body_size: " << instance._client_max_body_size;
	out << std::endl;

	out << "cgi_pass: " << instance._cgi_pass;
	out << std::endl;

	out << "cgi_param: " << instance._cgi_param;
	out << std::endl;

	out << "index: " << instance._index;
	out << std::endl;
	
	out << "autoindex: " << std::boolalpha << instance._autoindex;
	out << std::endl;

	return ( out );
}

/*
std::ostream &
operator << ( std::ostream & out, const ConfigServer & server )
{
	out << "______________ PRINTING THE MAP ________________" << std::endl;
	out << "server_name: ";

	for (size_t i = 0; i < server._server_name.size(); i++) {
		out << server._server_name[i];
		if (i != server._server_name.size() - 1)
			out << " ";
	}

    out << "Listen port:" << server._port << "  Listen host:" <<  server._host << std::endl;
	out << "client_body_buffer_size: " << server._client_max_body_size << std::endl;
	out << " root: " << server._root << std::endl;
    out << "mapa size: "<< server._location.size() << std::endl;
	for (std::map<std::string, ConfigServer>::const_iterator i = server._location.begin(); i != server._location.end(); i++) {
        out << std::endl << "Location string: " << i->first << std::endl;
		out << "dentro de location root: " << i->second.getRoot() << std::endl;
		out << "dentro de location maxsize: " << i->second.getClient_max_body_size() << std::endl;
	}

	return ( out );
}
*/
