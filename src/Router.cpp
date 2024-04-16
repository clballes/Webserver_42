/*                                                                            */
/* Router.cpp                                                                 */
/* mpuig-ma <mpuig-ma@student.42barcelona.com>                                */
/* Tue Apr 16 11:52:05 2024                                                   */

#include "Router.hpp"

Router::Router ( void ): _good( true )
{
	IEvent::kq = ::kqueue();
	DEBUG ( IEvent::kq );
	if ( IEvent::kq == -1 )
	{
		ERROR( "kqueue: " << ::strerror( errno ) );
		this->_good = false;
	}
	return ;
}

Router::~Router ( void )
{
	return ;
}

int
Router::setConnection ( struct sockaddr_in & address,
		int domain, int type, int protocol )
{
	this->_connections.push_back( Connection( address,
				domain, type, protocol ) );
	return ( EXIT_SUCCESS );
}

int
Router::listen ( void )
{
	int n_events = 1;
	struct kevent ev;
	IEvent * instance;

	DEBUG( IEvent::kq );
	for ( std::vector< Connection >::iterator i = this->_connections.begin();
			i != this->_connections.end(); ++i )
	{
		if ( this->register_read_socket( *i ) == EXIT_FAILURE )
			return ( EXIT_FAILURE );
	}
	// TODO: graceful_stop(), SIGINT
	while ( this->good() )
	{
		n_events = ::kevent( IEvent::kq, 0x0, 0, &ev, 1, 0 );
		if ( this->good() == false )
			return ( EXIT_FAILURE );
		if ( n_events == -1 )
		{
			ERROR( "kevent: " << ::strerror( errno ) );
			break ;
		}
		else if ( n_events == 0 )
			continue ;
		instance = static_cast< IEvent * >( ev.udata );
		instance->dispatch( ev );
		// TODO: consider EVFILT_SIGNAL
	}
	return ( EXIT_SUCCESS );
}

int
Router::register_read_socket ( Connection & instance ) const
{
	struct kevent ev;
	//static struct timespec ev_timeout;

	DEBUG( "fd=" << instance.getSocketFD() );
	EV_SET( &ev, instance.getSocketFD(), EVFILT_READ,
			EV_ADD | EV_ENABLE | EV_CLEAR, 0, 0, (void *) this );
	if ( ::kevent( IEvent::kq, &ev, 1, 0x0, 0, 0 ) == -1 )
	{
		ERROR( "kevent: " << ::strerror( errno ) );
		return ( EXIT_FAILURE );
	}
	return ( EXIT_SUCCESS );
}

void
Router::dispatch ( struct kevent & ev )
{
	HTTP * client;

	DEBUG ( "ev=" << ev.ident );
	if ( ev.filter != EVFILT_READ )
	{
		WARN( "unknown event filter" );
		return ;
	}
	client = new HTTP( *this, ev.ident );
	return ;
}

int
Router::setServer ( std::vector< std::string > & server_names )
{
	(void) server_names;
	return ( EXIT_SUCCESS );
}

bool
Router::good ( void ) const
{
	return ( this->_good );
}

Server *
Router::getServer ( std::string & server_name )
{
	DEBUG( server_name );
	if ( this->_servers.empty() )
		return ( nullptr );
	return ( &this->_servers[0] );
}
