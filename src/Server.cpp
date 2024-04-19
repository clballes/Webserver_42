/*                                                                            */
/* Server.cpp                                                                 */
/* mpuig-ma <mpuig-ma@student.42barcelona.com>                                */
/* Wed Apr  3 11:01:59 2024                                                   */

#include "Server.hpp"

Server::Server ( void ): _good( true )
{
	DEBUG ( "" );
	this->_error_pages[400] = "src/err_pages/400.html";
	this->_error_pages[403] = "src/err_pages/403.html";
	this->_error_pages[404] = "src/err_pages/404html";
	this->_error_pages[405] = "src/err_pages/405.html";
	this->_error_pages[410] = "src/err_pages/410.html";
	this->_error_pages[413] = "src/err_pages/413.html";
	this->_error_pages[500] = "src/err_pages/500.html";
	this->_routes[""].setDefault();
	return ;
}

Server &
Server::operator= ( const Server & instance )
{
	this->_good = instance._good;
	std::memcpy( &this->_address, &instance._address, sizeof( this->_address ) );
	this->_server_name = instance._server_name;
	this->_client_max_body_size = instance._client_max_body_size;
	this->_error_pages = instance._error_pages;
	this->_routes = instance._routes;
	return ( *this );
}

Server::~Server ( void )
{
	return ;
}

bool
Server::good ( void ) const
{
	return ( this->_good );
}

Location &
Server::getRoute ( std::string & location ) const
{
	t_route_map::const_iterator it;

	DEBUG( "\"" << location << "\"" );
	it = this->_routes.begin();
	while ( it != this->_routes.end() )
	{
		if ( it->first.compare( location ) == 0 )
		{
			DEBUG( "ret=" << it->first );
			return ( const_cast< Location & >( it->second ) );
		}
		++it;
	}
	return ( getDefaultRoute() );
}

Location &
Server::getDefaultRoute ( void ) const
{
	t_route_map::const_iterator it;

	it = this->_routes.begin();
	while ( it != this->_routes.end() )
	{
		if ( it->second.isDefault() == true )
			break ;
		++it;
	}
	if ( it == this->_routes.end() )
		it = this->_routes.begin();
	DEBUG( "ret=" << it->first );
	return ( const_cast< Location & >( it->second ) );
}

bool
Server::getFlag ( int mask, std::string location ) const
{
	return ( getRoute( location ).getFlag( mask ) );
}

std::size_t
Server::getFlags ( std::string location ) const
{
	return ( getRoute( location ).getFlags() );
}

std::size_t
Server::getClientMaxBodySize ( void ) const
{
	return ( this->_client_max_body_size );
}

const std::string &
Server::getCGIparam ( std::string location ) const
{
	return ( getRoute( location ).getCGIparam() );
}

const std::string &
Server::getCGIpass ( std::string location ) const
{
	return ( getRoute( location ).getCGIpass() );
}

const std::string &
Server::getRoot ( std::string location ) const
{
	return ( getRoute( location ).getRoot() );
}

std::vector< std::string > &
Server::getServerNames ( void ) const
{
	return ( const_cast< std::vector< std::string > & >( this->_server_name ) );
}

std::vector< std::string > &
Server::getIndex ( std::string location ) const
{
	return ( getRoute( location ).getIndex() );
}

// TODO: undefined behaviour control
const std::string &
Server::getErrorPage ( int errnum )
{
	return ( this->_error_pages[errnum] );
}

in_addr_t
Server::getHost ( void ) const
{
	return ( ntohl( this->_address.sin_addr.s_addr ) );
}

in_port_t
Server::getPort ( void ) const
{
	return ( ntohs( this->_address.sin_port ) );
}

const struct sockaddr_in &
Server::getListen ( void ) const
{
	return ( this->_address );
}

int
Server::setRoute ( std::string & location )
{
	DEBUG( location );
	if ( this->_routes.find( location ) == this->_routes.end() )
	{
		(void) this->_routes[location];
	}
	else
	{
		ERROR( "location \"" << location << "\" already set" );
		return ( EXIT_FAILURE );
	}
	return ( EXIT_SUCCESS );
}

int
Server::setFlag ( int flag, bool enable, std::string location )
{
	//DEBUG( flag );
	return ( getRoute( location ).setFlag( flag, enable ) );
}

int
Server::setClientMaxBodySize ( std::size_t size )
{
	// TODO: validate
	//DEBUG( size );
	this->_client_max_body_size = size;
	return ( EXIT_SUCCESS );
}

int
Server::setCGIparam ( std::string & arg, std::string location )
{
	//DEBUG( arg );
	return ( this->getRoute( location ).setCGIparam( arg ) );
}

int
Server::setCGIpass ( std::string & arg, std::string location )
{
	//DEBUG( arg );
	return ( this->getRoute( location ).setCGIpass( arg ) );
}

int
Server::setListen ( struct sockaddr_in & address )
{
	std::memcpy( &this->_address, &address, sizeof( address ) );
	#ifdef ALLOW_FORBIDDEN
	char str[INET_ADDRSTRLEN];
	inet_ntop( AF_INET, &this->_address.sin_addr.s_addr, str, INET_ADDRSTRLEN );
	//DEBUG( str << ':' << std::dec << ntohs( this->_address.sin_port ) );
	#endif
	return ( EXIT_SUCCESS );
}

int
Server::setRoot ( std::string & arg, std::string location )
{
	DEBUG( arg );
	DEBUG( "location=" << location );
	return ( this->getRoute( location ).setRoot( arg ) );
}

int
Server::setServerName ( std::string & arg )
{
	//DEBUG( arg );
	//TODO: check values
	if ( arg.empty() )
		return ( EXIT_FAILURE );
	if ( ! isalpha( arg[0] ) )
		return ( EXIT_FAILURE );
	for ( std::string::size_type i = 0; i < arg.length(); i++ )
	{
		if ( ! isalnum( arg[i] ) && arg[i] != '-' && arg[i] != '.' )
          	return ( EXIT_FAILURE );
	}
    this->_server_name.push_back( arg );
	return ( EXIT_SUCCESS );
}

int
Server::setIndex ( std::string & arg, std::string location )
{
	//DEBUG( arg );
	return ( this->getRoute( location ).setIndex( arg ) );
}

int
Server::setErrorPage ( int n, std::string & path )
{
	//DEBUG( "" );
	//TODO: check values
	if ( n < 400 || n > 555 )//perq exit failulre
		return ( EXIT_FAILURE ); 
	this->_error_pages[n] = path;
	return ( EXIT_SUCCESS );
}

void
Server::log_conf ( void ) const 
{
	LOG( "Server" );
	LOG( "good=" << std::boolalpha << this->good() );
	for ( std::vector< std::string >::const_iterator it = this->_server_name.begin();
			it != this->_server_name.end(); ++it )
		LOG( "server_name=" << *it );
	for ( std::map< int, std::string >::const_iterator it = this->_error_pages.begin();
			it != this->_error_pages.end(); ++it )
		LOG( "error_page=" << it->first << "=" << it->second );
	LOG( "client_max_body=" << this->_client_max_body_size );
	for ( t_route_map::const_iterator it = this->_routes.begin();
			it != this->_routes.end(); ++it )
	{
		LOG( "" );
		LOG( " Location \"" << it->first << "\"" );
		LOG( " root=" << it->second.getRoot() );
		LOG( " isDefault=" << it->second.isDefault() );
		LOG( " flags=" << std::hex << it->second.getFlags() << std::dec );
		LOG( " cgi_pass=" << it->second.getCGIpass() );
		LOG( " cgi_param=" << it->second.getCGIparam() );
		for ( std::vector< std::string >::const_iterator index_it = it->second.getIndex().begin();
				index_it != it->second.getIndex().end(); ++index_it )
			LOG( " index=" << *index_it );
	}
	return ;
}
