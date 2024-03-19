/*                                                                            */
/* events.cpp                                                                 */
/* mpuig-ma <mpuig-ma@student.42barcelona.com>                                */
/* Wed Feb 21 15:54:38 2024                                                   */

#include "Client.hpp"
#include "Server.hpp"

void
Server::dispatch ( struct kevent & ev )
{
	Client * c;

	LOG( "call Server::dispatch() (fd=" << ev.ident << ")" );

	c = new Client( *this );

	(void) c;
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
