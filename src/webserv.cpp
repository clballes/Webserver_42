/*                                                                            */
/* test-kqueue.cpp                                                            */
/* mpuig-ma <mpuig-ma@student.42barcelona.com>                                */
/* Thu Jan 25 14:26:11 2024                                                   */

#include "webserv.hpp"
#include "Server.hpp"

void graceful_stop ( int );
void set_events ( int );
void event_loop ( int );

// The webserv() call starts multiple Server instances 
// (...) using a kqueue.

void
webserv ( void )
{
	int kq;
	//bool status = true;

	// Configure SIGINT ( signal interrupt )
	// so as to finish connections and end the program
	// gracefully.
	
	::signal( SIGINT, &graceful_stop );

	// Create a new kernel event queue.

	kq = ::kqueue();
	if ( kq == -1 )
	{
		std::cerr << PROGRAM_NAME;
		std::cerr << ": " << ::strerror( errno ) << std::endl;
		exit ( 0x1 ); // Caldra comprovar leaks ...
	}
	
	LOG ( kq << ": kqueue properly initialized" << std::endl );

	// For each Server instance,
	// register an event with the queue.

	// WIP: maybe just set_init_events (sockets mainly)
	// ; there will be more events to register later,
	// during program execution and loop execution.

	set_events( kq );

	// Start listening for registered events
	// ... kqueue

	event_loop( kq );

	return ;
}

void
graceful_stop ( int n )
{
	(void) n;
	LOG ( "\rGracefully stoppping..." << std::endl );
	exit( n );
}
