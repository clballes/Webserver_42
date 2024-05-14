/*                                                                            */
/* Server.cpp                                                                 */
/* mpuig-ma <mpuig-ma@student.42barcelona.com>                                */
/* Wed Apr  3 11:01:59 2024                                                   */

#include "Server.hpp"

Server::Server ( void ): _good( true )
{
	std::memset( &this->_address, 0, sizeof( this->_address ) );
	this->_error_pages[400] = "src/err_pages/400.html";
	this->_error_pages[403] = "src/err_pages/403.html";
	this->_error_pages[404] = "src/err_pages/404.html";
	this->_error_pages[405] = "src/err_pages/405.html";
	this->_error_pages[410] = "src/err_pages/410.html";
	this->_error_pages[413] = "src/err_pages/413.html";
	this->_error_pages[500] = "src/err_pages/500.html";
	// in nginx the default value for client_max is 1MB, then 1,048,576bytes
	this->_routes[""].setDefault();
	this->getDefaultRoute().setRoot( "/html" );
	this->getDefaultRoute().setFlag( F_AUTOINDEX, false );
	return ;
}

Server &
Server::operator= ( const Server & instance )
{
	this->_good = instance._good;
	std::memcpy( &this->_address, &instance._address, sizeof( this->_address ) );
	this->_server_name = instance._server_name;
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

// Locations, a.k.a routes, are stored without ending `/'.

Location &
Server::getRoute ( const std::string & location ) const
{
	t_route_map::const_iterator		it;
	std::string						target( location );
	std::string						cmp_str;
	
	it = this->_routes.begin();
	while ( it != this->_routes.end() )
	{
		cmp_str.assign( it->first );
		if ( cmp_str[0] == '*' )
		{
			cmp_str.erase( 0, 1 );
			if ( cmp_str.length() > 1 && cmp_str.back() == '/' )
				cmp_str.erase( cmp_str.length() - 1, 1 );
			if ( compare_file_extension( target, cmp_str ) == true )
				return ( const_cast< Location & >( it->second ) );
		}
		++it;
	}
	it = this->_routes.begin();
	while ( it != this->_routes.end() )
	{
		cmp_str.assign( it->first );
		if ( target.compare( 0, cmp_str.length(), cmp_str ) == 0 )
			return ( const_cast< Location & >( it->second ) );
		++it;
	}
	return ( getDefaultRoute() );
}

// TODO: there is no option to set default route.

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
		--it;
	return ( const_cast< Location & >( it->second ) );
}

const std::string &
Server::getRouteString ( const std::string & location ) const
{
	t_route_map::const_iterator it;
	std::string cmp_str, target( location );

	it = this->_routes.begin();
	while ( it != this->_routes.end() )
	{
		cmp_str.assign( it->first );
		if ( cmp_str[0] == '*' )
		{
			cmp_str.erase( 0, 1 );
			if ( compare_file_extension( target, cmp_str ) == true )
				return ( const_cast< std::string & >( it->first ) );
		}
		++it;
	}
	it = this->_routes.begin();
	while ( it != this->_routes.end() )
	{
		cmp_str.assign( it->first );
		if ( target.compare( 0, cmp_str.length(), cmp_str ) == 0 )
			return ( const_cast< std::string & >( it->first ) );
		++it;
	}
	--it;
	return ( const_cast< std::string & >( (it->first)));
}

bool
Server::getFlag ( int mask, std::string location ) const
{
	const Location & loc = this->getRoute( location );
	bool flag = loc.getFlag( mask );
	return ( flag );
}

std::size_t
Server::getFlags ( std::string location ) const
{
	const Location & loc = this->getRoute( location );
	std::size_t flags = loc.getFlags();
	return ( flags );
}

const std::size_t &
Server::getClientMaxBodySize (  std::string location ) const
{
	const Location & loc = this->getRoute( location );
	const std::size_t & client_max_body_size = loc.getClientMaxBodySize();
	return ( client_max_body_size );
}

const std::pair<int, std::string> & Server::getRedirection ( std::string location ) const
{
	const Location & loc = this->getRoute( location );
	const std::pair<int, std::string> & redirection = loc.getRedirection();
	return ( redirection );
}

const std::string &
Server::getCGIpass ( std::string location ) const
{
	const Location & loc = this->getRoute( location );
	const std::string & cgi_pass = loc.getCGIpass();
	return ( cgi_pass );
}


const std::string &
Server::getRoot ( std::string location ) const
{
	const Location & loc = this->getRoute( location );
	const std::string & root = loc.getRoot();
	return ( root );
}

const std::vector< std::string > &
Server::getServerNames ( void ) const
{
	return ( this->_server_name );
}

bool
Server::hasServerName ( std::string & name ) const
{
	std::vector< std::string >::const_iterator it;

	it = this->_server_name.begin();
	while ( it != this->_server_name.end() )
	{
		if ( it->compare( name ) == 0 )
			return ( true );
		++it;
	}
	return ( false );
}

const std::vector< std::string > &
Server::getIndex ( std::string location ) const
{
	return ( getRoute( location ).getIndex() );
}

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
Server::setRoute ( const std::string & location )
{
	std::string mod_location( location );

	while ( mod_location.back() == '/' )
		mod_location.erase( mod_location.length() - 1, 1 );
	if ( mod_location.back() != '/' ) 
		mod_location.append( "/" );
	if ( mod_location.size() > 1 && mod_location[0] == '*' )
		mod_location.erase( mod_location.length() - 1, 1 );
	for ( t_route_map::const_iterator it = this->_routes.begin();
			it != this->_routes.end(); ++it )
	{
		if ( it->first == mod_location )
		{
			ERROR( "location \"" << mod_location << "\" already set" );
			return ( EXIT_FAILURE );
		}
	}
	(void) this->_routes[mod_location];
	return ( EXIT_SUCCESS );
}

int
Server::setFlag ( int flag, bool enable, std::string location )
{
	return ( getRoute( location ).setFlag( flag, enable ) );
}

int
Server::setClientMaxBodySize ( const std::string & arg, std::string location )
{
	return ( this->getRoute( location ).setClientMaxBodySize( arg ) );
}

int
Server::setCGIpass ( const std::string & arg, std::string location )
{
	return ( this->getRoute( location ).setCGIpass( arg ) );
}

int
Server::setListen ( struct sockaddr_in & address )
{
	std::memcpy( &this->_address, &address, sizeof( address ) );
	return ( EXIT_SUCCESS );
}

int
Server::setRoot ( const std::string & arg, std::string location )
{
	return ( this->getRoute( location ).setRoot( arg ) );
}

int
Server::setServerName ( const std::string & arg )
{
	if ( arg.empty() )
		return ( EXIT_FAILURE );
	if ( ! isalpha( arg[0] ) )
		return ( EXIT_FAILURE );
	for ( std::string::size_type i = 0; i < arg.length(); i++ )
	{
		if ( ! isalnum( arg[i] )
				&& arg[i] != '-'
				&& arg[i] != '.' )
          	return ( EXIT_FAILURE );
	}
    this->_server_name.push_back( arg );
	return ( EXIT_SUCCESS );
}

int
Server::setIndex ( const std::string & arg, std::string location )
{
	return ( this->getRoute( location ).setIndex( arg ) );
}

int
Server::setErrorPage ( int n, const std::string & path )
{
	if ( n < 400 || n > 555 )
	{
		ERROR( "error \"" << n << "\" not contempled" );
		return ( EXIT_FAILURE );
	}
	this->_error_pages[n] = path;
	return ( EXIT_SUCCESS );
}

int
Server::setRedirection ( const std::string & arg, std::string location )
{
	
	return ( this->getRoute( location ).setRedirection( arg ) );
}

int
Server::check ( void )
{
	if ( this->_server_name.empty() == true )
		this->_server_name.push_back( "" );
	if ( this->getPort() == 0 && this->getHost() == 0
			&& this->_address.sin_family == 0 )
	{
		ERROR( "directive \"listen\" required" );
		return ( EXIT_FAILURE );
	}
	if ( this->getDefaultRoute().getRoot().empty() == true )
	{
		ERROR( "directive \"root\" required" );
		return ( EXIT_FAILURE );
	}

	for ( t_route_map::const_iterator it = this->_routes.begin();
			it != this->_routes.end(); ++it )
	{
		if ( it->second.getRoot().empty() )
		{
			ERROR( "directive \"root\" required in location "
				<< "\"" << it->first << "\"" );
			return ( EXIT_FAILURE );
		}
	}

	return ( EXIT_SUCCESS );
}
