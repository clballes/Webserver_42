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

	DEBUG( " ********************* PETES????? RATA" );
	it = this->_routes.begin();
	while ( it != this->_routes.end() )
	{
		if ( it->first.compare( location ) == 0 )
			return ( const_cast< Location & >( it->second ) );
		++it;
	}
	return ( getDefaultRoute() );
}

Location &
Server::getDefaultRoute ( void ) const
{
	t_route_map::const_iterator it;

	DEBUG( "********************* aixo esciurssssssss?????" );
	it = this->_routes.begin();
	while ( it != this->_routes.end() )
	{
		if ( it->second.isDefault() == true )
			break ;
		++it;
	}
	if ( it == this->_routes.end() )
		it = this->_routes.begin();
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
		(void) this->_routes[location];
	return ( EXIT_SUCCESS );
}

int
Server::setFlag ( int flag, bool enable, std::string location )
{
	DEBUG( flag );
	return ( getRoute( location ).setFlag( flag, enable ) );
}

int
Server::setClientMaxBodySize ( std::size_t size )
{
	// TODO: validate
	DEBUG( size );
	this->_client_max_body_size = size;
	return ( EXIT_SUCCESS );
}

int
Server::setCGIparam ( std::string & arg, std::string location )
{
	DEBUG( arg );
	return ( this->getRoute( location ).setCGIparam( arg ) );
}

int
Server::setCGIpass ( std::string & arg, std::string location )
{
	DEBUG( arg );
	return ( this->getRoute( location ).setCGIpass( arg ) );
}

int
Server::setListen ( struct sockaddr_in & address )
{
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
	DEBUG( arg );
	return ( this->getRoute( location ).setRoot( arg ) );
}

int
Server::setServerName ( std::string & arg )
{
	DEBUG( arg );
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
	DEBUG( arg );
	return ( this->getRoute( location ).setIndex( arg ) );
}

int
Server::setErrorPage ( int n, std::string & path )
{
	LOG("call set_error_page()");

  	std::string pathStr( path );
  	std::string uri;
	int number;

    for ( std::size_t i = 0; i <= pathStr.length(); i++ )
	{
		if ( std::isdigit( path[i] ) )
		{
			if ( path[i + 1] == 32 )
			{
				std::string numberStr = pathStr.substr( 0, i + 1 );
				number = std::atoi( numberStr.c_str() );
				i++;

				if ( path[i + 1] == '/' )
				{
					uri = pathStr.substr( i + 1, pathStr.length() );
					std::string uri_complet = DEFAULT_ERROR_DIR + uri;
					this->_error_pages[number] = uri_complet;
					return ( EXIT_SUCCESS );
				}
				else
					return ( EXIT_FAILURE );
			}
		}
		else
			return ( EXIT_FAILURE );
    }
	return ( EXIT_SUCCESS );





	//TODO: check values
	if ( n < 400 || n > 555 )
		return ( EXIT_FAILURE );
	std::cout << " //////////////////////////// NO ET RALLSI AQUI PORFA"<< "INT N: "<< n << " path : " << path << std::endl;

	
	// std::map<int, std::string>::iterator it = _error_pages.begin();
    // std::map<int, std::string>::iterator it2 = _error_pages.end();
    
    // for (; it != it2; ++it) {
    //     std::cout << "Key: " << it->first << ", Value: " << it->second << std::endl;
    // }
	this->_error_pages[n] = path;
	std::cout << " //////////////////////////// NO ET RALLSI AQUI PORFA"<< "INT N: "<< n << " path : " << path << std::endl;

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
		LOG( "root=" << it->second.getRoot() );
		LOG( "isDefault=" << it->second.isDefault() );
		LOG( "flags=" << it->second.getFlags() );
		LOG( "cgi_pass=" << it->second.getCGIpass() );
		LOG( "cgi_param=" << it->second.getCGIparam() );
		for ( std::vector< std::string >::iterator index_it = it->second.getIndex().begin();
				index_it != it->second.getIndex().end(); ++it )
			LOG( "index=" << *index_it );
	}
	return ;
}
