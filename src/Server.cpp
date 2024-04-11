/*                                                                            */
/* Server.cpp                                                                 */
/* mpuig-ma <mpuig-ma@student.42barcelona.com>                                */
/* Wed Apr  3 11:01:59 2024                                                   */

#include "Server.hpp"

Server::Server ( void ): _good( true ), _socket_fd( 0 ) // TODO: _flags( 0 )
{
	//DEBUG ( "" );
	//TODO: find out why cannot bind if socket is created in constructor
	//instead of start();
	return ;
}

Server::~Server ( void )
{
	if ( this->_socket_fd != 0 )
		close( this->_socket_fd );
	return ;
}

bool
Server::good ( void ) const
{
	return ( this->_good );
}

int
Server::id ( void ) const
{
	return ( this->_socket_fd );
}

int
Server::start ( void )
{
	// Create socket ( server's )
	this->_socket_fd = ::socket( AF_INET, SOCK_STREAM, IPPROTO_TCP );
	if ( this->_socket_fd == -1 || fcntl( this->_socket_fd,
			F_SETFL, O_NONBLOCK, FD_CLOEXEC ) == -1 )
	{
		ERROR( "socket: " << ::strerror( errno ) );
		this->_good = false;
		return ( EXIT_FAILURE );
	}
	#ifdef REUSE_SOCKET
	int enable = 1;
	if ( setsockopt( this->_socket_fd, SOL_SOCKET, SO_REUSEADDR,
				&enable, sizeof( enable ) ) == -1
			|| setsockopt( this->_socket_fd, SOL_SOCKET, SO_REUSEPORT,
				&enable, sizeof( enable ) ) == -1 )
	{
		ERROR( "socket: " << ::strerror( errno ) );
		this->_good = false;
		return ( EXIT_FAILURE );
	}
	#endif
	// Bind server's address to newly created socket.
	if ( ::bind( this->_socket_fd, (struct sockaddr *) &this->_address,
				sizeof( this->_address ) ) == -1 )
	{
		ERROR( "bind: " << ::strerror( errno )
				<< " (fd=" << this->_socket_fd << ")" );
		return ( EXIT_FAILURE );
	}
	// Start listening on server's address through binded socket.
	if ( ::listen( this->_socket_fd, 0x0 ) == -1 )
	{
		ERROR( "listen: " << ::strerror( errno )
				<< " (fd=" << this->_socket_fd << ")" );
		return ( EXIT_FAILURE );
	}
	#ifdef ALLOW_FORBIDDEN
	#include <arpa/inet.h>
	char str[INET_ADDRSTRLEN];
	inet_ntop( AF_INET, &this->_address.sin_addr.s_addr, str, INET_ADDRSTRLEN );
	DEBUG( str << ':' << std::dec << ntohs( this->_address.sin_port ) );
	#endif
	//TODO:
	this->register_read_socket();
	return ( EXIT_SUCCESS );
}

int
Server::stop ( void )
{
	return ( EXIT_SUCCESS );
}

void
Server::dispatch ( struct kevent & ev )
{
	HTTP * client;

	DEBUG( "ev=" << ev.ident );
	client = new HTTP( *this );
	(void) ev;	
	return ;
}

void
Server::register_read_socket ( void ) const
{
	struct kevent ev;
	//static struct timespec ev_timeout;

	DEBUG( "fd=" << this->_socket_fd );
	EV_SET( &ev, this->_socket_fd, EVFILT_READ,
			EV_ADD | EV_ENABLE | EV_CLEAR, 0, 0, (void * ) this );
	if ( ::kevent( IEvent::kq, &ev, 1, 0x0, 0, 0 ) == -1 )
		std::cerr << "kevent: " << ::strerror( errno ) << std::endl;
	return ;
}

// getLocation will try to match `location'
// else it will return the default server
// else it will return the last server
// TODO: _routes vector will have to have been sorted at some point std::sort	

const t_location &
Server::getLocation ( std::string location ) const
{
	std::vector< t_location >::const_iterator it, def;

	it = this->_routes.begin();
	def = this->_routes.end();
	while ( it != this->_routes.end() )
	{
		if ( location.compare( 0, it->root.length(), it->root ) == 0 )
		{
			LOG( "return=" << &(*it) );
			return ( *it );
		}
		if ( it->isDefault == true )
			def = it;
		++it;
	}
	if ( def != this->_routes.end() )
	{
		LOG( "return=" << &(*def) );
		return ( *def );
	}
	return ( this->_routes.back() );
}

std::vector< t_location >::iterator
Server::findLocation ( std::string location )
{
	std::vector< t_location >::iterator it;

	it = this->_routes.begin();
	while ( it != this->_routes.end() )
	{
		DEBUG( "location=" << location << location.length() );
		DEBUG( "root=" << it->root << it->root.length() );
		DEBUG( "current=" << &(*it) );
		if ( location.empty() && it->root.empty() )
			break ;
		if ( location.compare( 0, it->root.length(), it->root ) == 0 )
			break ;
		++it;
	}
	DEBUG( "returning" << &(*it) );
	return ( it );
}

int
Server::getSocketFD ( void ) const
{
	return ( this->_socket_fd );
}

bool
Server::getFlag ( int mask, std::string location ) const
{
	if ( getLocation( location ).flags & mask )
		return ( true );
	return ( false );
}

std::size_t
Server::getFlags ( std::string location ) const
{
	return ( getLocation( location ).flags );
}

std::size_t
Server::getClientMaxBodySize ( void ) const
{
	return ( this->_client_max_body_size );
}

const std::string &
Server::getCGIparam ( std::string location ) const
{
	return ( getLocation( location ).cgi_param );
}

const std::string &
Server::getCGIpass ( std::string location ) const
{
	return ( getLocation( location ).cgi_pass );
}

const std::string &
Server::getRoot ( std::string location ) const
{
	DEBUG( getLocation( location ).root.c_str() );
	return ( getLocation( location ).root );
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
	return ( getLocation( location ).index.begin() );
}

const std::string &
Server::getErrorPage ( int errnum )
{
	return ( this->_error_pages[errnum] );
}

std::vector< t_location >::iterator
Server::setLocation ( std::string location )
{
	std::vector< t_location >::iterator it;

	DEBUG( location.c_str() );
	it = findLocation( location );
	if ( it != this->_routes.end() )
		return ( it );
	DEBUG( "created new=" << location.c_str() );
	this->_routes.resize( this->_routes.size() + 1 );
	it = this->_routes.end() - 1;
	it->root = location;
	return ( it );
}

int
Server::setFlag ( int flag, bool enable, std::string location )
{
	std::vector< t_location >::iterator it;

	DEBUG( location.c_str() );
	it = this->findLocation( location );
	if ( it == this->_routes.end() )
		it = this->setLocation( location );
	if ( enable == true )
		it->flags |= flag;
	else
		it->flags ^= flag;
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
	std::vector< t_location >::iterator it;

	it = this->findLocation( location );
	if ( it == this->_routes.end() )
		it = this->setLocation( location );
	//TODO: check if multiple params ( which should be invalid )
	//TODO: check arg is valid
	it->cgi_param = arg;
	return ( EXIT_SUCCESS );
}

int
Server::setCGIpass ( std::string & arg, std::string location )
{
	std::vector< t_location >::iterator it;

	it = this->findLocation( location );
	if ( it == this->_routes.end() )
		it = this->setLocation( location );
	//TODO: check arg is valid
	//return ( EXIT_FAILURE );
	//DEBUG( arg.c_str() );
	it->cgi_pass = arg;
	return ( EXIT_SUCCESS );
}

int
Server::setListen( struct sockaddr_in & address )
{
	std::memcpy( &this->_address, &address, sizeof( address ) );
	/*
	#ifdef ALLOW_FORBIDDEN
	char str[INET_ADDRSTRLEN];
	inet_ntop( AF_INET, &this->_address.sin_addr.s_addr, str, INET_ADDRSTRLEN );
	DEBUG( str << ':' << std::dec << ntohs( this->_address.sin_port ) );
	#endif
	*/
	(void) _sockaddr_len;
	return ( EXIT_SUCCESS );
}

int
Server::setRoot ( std::string & arg, std::string location )
{
	std::vector< t_location >::iterator it;

	it = this->findLocation( location );
	if ( it == this->_routes.end() )
		it = this->setLocation( location );
	//TODO: check arg is valid
	//return ( EXIT_FAILURE );
	//DEBUG( arg.c_str() );
	it->root = arg;
	while ( it->root.back() == '/' )
		it->root.erase( it->root.length() - 1, 1 );
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
	std::vector< t_location >::iterator it;

	it = this->findLocation( location );
	if ( it == this->_routes.end() )
		it = this->setLocation( location );
	//TODO: check value
	//return ( EXIT_FAILURE );
	//DEBUG( arg.c_str() );
	if ( std::find( it->index.begin(),
				it->index.end(), arg ) == it->index.end() )
		it->index.push_back( arg );
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
	LOG( "Server fd=" << this->_socket_fd );
	#ifdef ALLOW_FORBIDDEN
	char str[INET_ADDRSTRLEN];
	inet_ntop( AF_INET, &this->_address.sin_addr.s_addr, str, INET_ADDRSTRLEN );
	LOG( str << ':' << std::dec << ntohs( this->_address.sin_port ) );
	#endif
	LOG( "good=" << std::boolalpha << this->good() );
	LOG( "root=" << this->getRoot() );
	LOG( "autoindex=" << std::boolalpha << this->getFlag( F_AUTOINDEX ) );
	LOG( "GET=" << std::boolalpha << this->getFlag( METHOD_GET ) );
	LOG( "POST=" << std::boolalpha << this->getFlag( METHOD_POST ) );
	LOG( "PUT=" << std::boolalpha << this->getFlag( METHOD_PUT ) );
	LOG( "DELETE=" << std::boolalpha << this->getFlag( METHOD_DELETE ) );
	LOG( "HEAD=" << std::boolalpha << this->getFlag( METHOD_HEAD ) );
	LOG( "cgi_param=" << this->getCGIparam() );
	LOG( "cgi_pass=" << this->getCGIpass() );
	for ( std::vector< std::string >::iterator it = this->_server_name.begin();
			it != this->_server_name.end(); ++it )
		LOG( "server_name=" << *it );
	/*
	for ( std::vector< std::string >::iterator it = this->_index.begin();
			it != this->_index.end(); ++it )
		LOG( "index=" << *it );
	*/
	return ;
}
