/*                                                                            */
/* Server.cpp                                                                 */
/* mpuig-ma <mpuig-ma@student.42barcelona.com>                                */
/* Wed Apr  3 11:01:59 2024                                                   */

#include "Server.hpp"

Server::Server ( void ): _good( true ) // TODO: _flags( 0 )
{
	DEBUG ( "" );
	//TODO: find out why cannot bind if socket is created in constructor
	//instead of start();
	// Set a default route
	this->_routes[""].isDefault = true;
	return ;
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

t_location &
Server::getRoute ( std::string & location ) const
{
	t_route_map::const_iterator it;

	it = this->_routes.begin();
	while ( it != this->_routes.end() )
	{
		if ( it->first.compare( location ) == 0 )
			return ( (t_location &) it->second );
		++it;
	}
	return ( (t_location &) getDefaultRoute() );
}

t_location &
Server::getDefaultRoute ( void ) const
{
	t_route_map::const_iterator it;

	it = this->_routes.begin();
	while ( it != this->_routes.end() )
	{
		if ( it->second.isDefault == true )
			break ;
		++it;
	}
	if ( it == this->_routes.end() )
		it = this->_routes.begin();
	return ( (t_location &) it->second );
}

bool
Server::getFlag ( int mask, std::string location ) const
{
	if ( getRoute( location ).flags & mask )
		return ( true );
	return ( false );
}

std::size_t
Server::getFlags ( std::string location ) const
{
	return ( getRoute( location ).flags );
}

std::size_t
Server::getClientMaxBodySize ( void ) const
{
	return ( this->_client_max_body_size );
}

const std::string &
Server::getCGIparam ( std::string location ) const
{
	return ( getRoute( location ).cgi_param );
}

const std::string &
Server::getCGIpass ( std::string location ) const
{
	return ( getRoute( location ).cgi_pass );
}

const std::string &
Server::getRoot ( std::string location ) const
{
	DEBUG( getRoute( location ).root.c_str() );
	return ( getRoute( location ).root );
}

// TODO: consider returning a const iterator instead
t_vector::const_iterator
Server::getServerNames ( void ) const
{
	return ( this->_server_name.begin() );
}

t_vector::const_iterator
Server::getIndex ( std::string location ) const
{
	return ( getRoute( location ).index.begin() );
}

const std::string &
Server::getErrorPage ( int errnum )
{
	return ( this->_error_pages[errnum] );
}

int
Server::setRoute ( std::string & location )
{
	if ( this->_routes.find( location ) == this->_routes.end() )
		(void) this->_routes[location];
	return ( EXIT_SUCCESS );
}

int
Server::setFlag ( int flag, bool enable, std::string location )
{
	DEBUG( location.c_str() );
	if ( enable == true )
		getRoute( location ).flags |= flag;
	else
		getRoute( location ).flags ^= flag;
	return ( EXIT_SUCCESS );
}

int
Server::setClientMaxBodySize ( std::size_t size )
{
	//DEBUG( std::dec << size );
	this->_client_max_body_size = size;
	return ( EXIT_SUCCESS );
}

int
Server::setCGIparam ( std::string & arg, std::string location )
{
	(void) arg;
	(void) location;
	//TODO: check if multiple params ( which should be invalid )
	//TODO: check arg is valid
	//it->cgi_param = arg;
	return ( EXIT_SUCCESS );
}

int
Server::setCGIpass ( std::string & arg, std::string location )
{
	(void) arg;
	(void) location;
	//it = this->findLocation( location );
	//if ( it == this->_routes.end() )
	//	it = this->setLocation( location );
	//TODO: check arg is valid
	//return ( EXIT_FAILURE );
	//DEBUG( arg.c_str() );
	//it->cgi_pass = arg;
	return ( EXIT_SUCCESS );
}

const struct sockaddr_in &
Server::getListen ( void ) const
{
	return ( this->_address );
}

int
Server::setListen ( struct sockaddr_in & address )
{
	(void) address;
	std::memcpy( &this->_address, &address, sizeof( address ) );
	#ifdef ALLOW_FORBIDDEN
	char str[INET_ADDRSTRLEN];
	inet_ntop( AF_INET, &this->_address.sin_addr.s_addr, str, INET_ADDRSTRLEN );
	DEBUG( str << ':' << std::dec << ntohs( this->_address.sin_port ) );
	#endif
	return ( EXIT_SUCCESS );
}

int
Server::setRoot ( std::string & arg, std::string location )
{
	(void) arg;
	(void) location;
	//TODO: check arg is valid
	//return ( EXIT_FAILURE );
	//DEBUG( arg.c_str() );
	/*
	it->root = arg;
	while ( it->root.back() == '/' )
		it->root.erase( it->root.length() - 1, 1 );
	*/
	return ( EXIT_SUCCESS );
}

int
Server::setServerName ( std::string & arg )
{
	//TODO: check values
	//return ( EXIT_FAILURE );
	//DEBUG( arg.c_str() );
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
	(void) arg;
	(void) location;
	//TODO: check value
	//return ( EXIT_FAILURE );
	//DEBUG( arg.c_str() );
	/*
	if ( std::find( it->index.begin(),
				it->index.end(), arg ) == it->index.end() )
		it->index.push_back( arg );
	*/
	return ( EXIT_SUCCESS );
}

int
Server::setErrorPage ( int n, std::string & path )
{
	//TODO: check values
	//return ( EXIT_FAILURE );
	//DEBUG( n << " " << path );
	if ( n < 400 || n > 555 )
		return ( EXIT_FAILURE );
	this->_error_pages[n] = path;
	return ( EXIT_SUCCESS );
}

void
Server::log_conf ( void ) 
{
	LOG( "Server" );
	LOG( "good=" << std::boolalpha << this->good() );
	for ( std::vector< std::string >::iterator it = this->_server_name.begin();
			it != this->_server_name.end(); ++it )
		LOG( "server_name=" << *it );
	for ( std::map< int, std::string >::iterator it = this->_error_pages.begin();
			it != this->_error_pages.end(); ++it )
		LOG( "error_page=" << it->first << "=" << it->second );
	LOG( "client_max_body=" << this->_client_max_body_size );
	for ( t_route_map::iterator it = this->_routes.begin();
			it != this->_routes.end(); ++it )
	{
		LOG( "root=" << it->second.root );
		LOG( "isDefault=" << it->second.isDefault );
		LOG( "flags=" << it->second.flags );
		LOG( "cgi_pass=" << it->second.cgi_pass );
		LOG( "cgi_param=" << it->second.cgi_param );
		for ( std::vector< std::string >::iterator index_it = it->second.index.begin();
				index_it != it->second.index.end(); ++it )
			LOG( "index=" << *index_it );
	}
	return ;
}
