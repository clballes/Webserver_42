/*                                                                            */
/* Server.cpp                                                                 */
/* mpuig-ma <mpuig-ma@student.42barcelona.com>                                */
/* Wed Apr  3 11:01:59 2024                                                   */

#include "Server.hpp"

Server::Server ( void ): _good( true ), _socket_fd( 0 )
{
	DEBUG ( "" );
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
	DEBUG( "OK " << this->id() );
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

	DEBUG( ev.ident );
	client = new HTTP( *this );
	(void) ev;	
	return ;
}

// not being used ... ?

void
Server::register_read_socket ( void ) const
{
	struct kevent ev;
	//static struct timespec ev_timeout;
	
	LOG( "call Server::register_read() (fd=" << this->_socket_fd << ")" );
	EV_SET( &ev, this->_socket_fd, EVFILT_READ,
			EV_ADD | EV_ENABLE | EV_CLEAR, 0, 0, (void * ) this );
	if ( ::kevent( IEvent::kq, &ev, 1, 0x0, 0, 0 ) == -1 )
		std::cerr << "kevent: " << ::strerror( errno ) << std::endl;
	return ;
}

// not being used ... ?

int
Server::receive_request ( int64_t data )
{
	( void ) data;
	/*
	char buffer[1024];

	LOG( "call Server::receive_request() (fd=" << this->_client_socket_fd << ")" );

	std::memset( buffer, '\0', data );
	ssize_t n = recv( this->_client_socket_fd, buffer, 1024, 0 );
	LOG( "n: " << n );

	write( STDOUT_FILENO, buffer, data );
	*/
	return ( EXIT_SUCCESS );
}


ServerConf &
Server::getConf ( void )
{
	return ( this->_conf );
}

int
Server::getSocketFD ( void ) const
{
	return ( this->_socket_fd );
}

#ifdef ALLOW_FORBIDDEN

//TODO: forbidden!
#include <arpa/inet.h>

int
Server::setListen( struct sockaddr_in & address )
{
	char str[INET_ADDRSTRLEN];

	std::memcpy( &this->_address, &address, sizeof( address ) );
	inet_ntop( AF_INET, &this->_address.sin_addr.s_addr, str, INET_ADDRSTRLEN );
	DEBUG( str << ':' << std::dec << ntohs( this->_address.sin_port ) );
	(void) _sockaddr_len;
	return ( EXIT_SUCCESS );
}

#else

int
Server::setListen( struct sockaddr_in & address )
{
	std::memcpy( &this->_address, &address, sizeof( address ) );
	(void) _sockaddr_len;
	return ( EXIT_SUCCESS );
}

#endif
