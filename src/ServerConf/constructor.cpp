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

ServerConf::ServerConf ( void ):
	good( true ),
	_root("html"),
	_allow_methods(0),
	_client_max_body_size(0),
	_index(1, "index.html")
{
	LOG( "call ServerConf::ServerConf( void )" );
	
	this->_error_page[400].assign( DEFAULT_ERROR_DIR ).append( "/400.html" );
	this->_error_page[403].assign( DEFAULT_ERROR_DIR ).append( "/403.html" );
	this->_error_page[404].assign( DEFAULT_ERROR_DIR ).append( "/404.html" );
	this->_error_page[405].assign( DEFAULT_ERROR_DIR ).append( "/405.html" );
	this->_error_page[410].assign( DEFAULT_ERROR_DIR ).append( "/410.html" );
	this->_error_page[413].assign( DEFAULT_ERROR_DIR ).append( "/413.html" );
	this->_error_page[500].assign( DEFAULT_ERROR_DIR ).append( "/500.html" );
	this->_address.sin_port = 80;
	return ;
}

ServerConf::ServerConf ( const ServerConf & instance )
{
	*this = instance;
	
	LOG( "call ServerConf::ServerConf( const ServerConf & )" );
	
	return ;
}

ServerConf::ServerConf ( const std::deque< std::string > & server_block ):
	good( true ),
	_root("html"),
	_allow_methods(0), 
	_client_max_body_size(0),
	_index(1, "index.html")
{
	LOG( "call ServerConf( const std::deque< std::string > &" )

	this->_error_page[400].assign( DEFAULT_ERROR_DIR ).append( "/400.html" );
	this->_error_page[403].assign( DEFAULT_ERROR_DIR ).append( "/403.html" );
	this->_error_page[404].assign( DEFAULT_ERROR_DIR ).append( "/404.html" );
	this->_error_page[405].assign( DEFAULT_ERROR_DIR ).append( "/405.html" );
	this->_error_page[410].assign( DEFAULT_ERROR_DIR ).append( "/410.html" );
	this->_error_page[413].assign( DEFAULT_ERROR_DIR ).append( "/413.html" );
	this->_error_page[500].assign( DEFAULT_ERROR_DIR ).append( "/500.html" );
	this->_address.sin_port = 80;

	if ( ServerConf::set_directives( server_block ) == EXIT_FAILURE )
	{
		std::cerr << "check_directives" << std::endl;
		this->good = ! good;
	}
	return ;
}

ServerConf &
ServerConf::operator = ( const ServerConf & instance )
{
	this->good = instance.good;
	// WIP

	LOG( "call ServerConf::operator=ServerConf" );

	return ( *this );
}

std::ostream &
operator << ( std::ostream & out, const ServerConf & instance )
{
	std::vector< std::string >::const_iterator it;

	out << " listen: " << inet_ntoa( instance._address.sin_addr );              // @mpuig-ma
	out << " port: " << ntohs( instance._address.sin_port );
	out << std::endl;

	out << " server_name:";
	for ( it = instance._server_name.begin();
			it != instance._server_name.end(); ++it )
		out << " " << *it;
	out << std::endl;

	out << " root: " << instance._root;
	out << std::endl;

	/*
	out << "Allowed methods: Bitwise representation: ";
	for ( int i = 6; i >= 0; --i )
		out << ( (instance._allow_methods & ( 1 << i ) ) ? '1' : '0' );
	out << std::endl;
	*/

    out << " allowed_methods:";
    if ( instance._allow_methods & METHOD_GET )
        out << " GET";
    if ( instance._allow_methods & METHOD_POST )
        out << " POST";
    if ( instance._allow_methods & METHOD_PUT )
        out << " PUT";
    if ( instance._allow_methods & METHOD_DELETE )
        out << " DELETE";
    out << std::endl;

	out << " client_max_body_size: " << instance._client_max_body_size;
	out << std::endl;

	out << " cgi_pass: " << instance._cgi_pass;
	out << std::endl;

	out << " cgi_param: " << instance._cgi_param;
	out << std::endl;

	if ( instance._allow_methods & F_AUTOINDEX )
        out << " autoindex: on" << std::endl;
	
	out << " index:";
	for ( it = instance._index.begin();
			it != instance._index.end(); ++it )
		out << " " << *it;

	/*
    out << "Error Pages:";
	std::map<int, std::string>::const_iterator it2 = instance._error_page.begin();
	while ( it2 != instance._error_page.end() )
	{
		out << "Int: " << it2->first << ", Value String: " << it2->second;
		out << std::endl;
		++it2;
	}
	*/

	return ( out );
}
