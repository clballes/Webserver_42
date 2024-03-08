/*                                                                            */
/* test-kqueue.cpp                                                            */
/* mpuig-ma <mpuig-ma@student.42barcelona.com>                                */
/* Thu Jan 25 14:26:11 2024                                                   */

#include "IEvent.hpp"
#include "webserv.hpp"
#include "Server.hpp"

void graceful_stop ( int );
void set_events ( int );
void event_loop ( int );

// The webserv() call starts multiple Server instances 
// (...) using a kqueue.

bool status;

int
webserv ( void )
{
	LOG( "call webserv()" );

	// Configure SIGINT ( signal interrupt )
	// so as to finish connections and end the program
	// gracefully.
	
	::signal( SIGINT, &graceful_stop );
	::signal( SIGQUIT, &graceful_stop );

	// Create a new kernel event queue.

	IEvent::kq = ::kqueue();
	if ( IEvent::kq == -1 )
	{
		std::cerr << PROGRAM_NAME;
		std::cerr << ": kqueue: " << ::strerror( errno );
		std::cerr << std::endl;
		exit ( 0x1 ); // Caldra comprovar leaks ...
	}
	
	LOG ( " kqueue properly initialized (fd=" << IEvent::kq << ")" );

	// For each Server instance,
	// register an event with the queue.

	// WIP: maybe just set_init_events (sockets mainly)
	// ; there will be more events to register later,
	// during program execution and loop execution.

	for ( Server::iterator it = Server::servers.begin();
			it != Server::servers.end(); ++it )
	{
		if ( ( *it )->start() )
		{
			// WIP: retry once after ~30 seconds.
			return ( EXIT_FAILURE );
		}
	}

	// Start listening for registered events
	// ... kqueue

	event_loop( IEvent::kq );

	return ( EXIT_SUCCESS );
}

void
graceful_stop ( int n )
{
	LOG( "" );
	LOG( "Stopping " << PROGRAM_NAME << " gracefully (s=" << n << ")" );
	status = false;

	for ( Server::iterator it = Server::servers.begin();
		it != Server::servers.end(); ++it )
		( *it )->stop();

	close( IEvent::kq );

	return ;
}
