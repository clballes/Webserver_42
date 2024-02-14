/*                                                                            */
/* test-kqueue.cpp                                                            */
/* mpuig-ma <mpuig-ma@student.42barcelona.com>                                */
/* Thu Jan 25 14:26:11 2024                                                   */

#include "webserv.hpp"
#include "Server.hpp"

#define LOG(s) { std::clog << s << std::endl; }

void
graceful_stop (int n)
{
	(void) n;
	std::clog << "\rGracefully stoppping..." << std::endl;
	sleep(1);
}

// The webserv() call starts multiple Server instances 
// (...) using a kqueue.

void
webserv ( const std::set< const Server * > & servers )
{
	// Configure SIGINT ( signal interrupt )
	// so as to finish connections and end the program
	// gracefully.
	
	::signal( SIGINT, &graceful_stop );

	static struct timespec timeout = { 1, 0 };
	struct kevent ev = {};
	int fd, kq, n_events;

	(void)timeout; (void) fd; (void)ev; (void)kq; (void)n_events;
	(void)servers;

	// Create a new kernel event queue.

	kq = ::kqueue();
	if ( kq == -1 )
	{
		std::cerr << PROGRAM_NAME;
		std::cerr << ": " << ::strerror( errno ) << std::endl;
		exit (0x1); // Caldra comprovar leaks ...
	}

	// Register an event with the queue for each Server instance.



	std::clog << kq << ": kqueue properly initialized" << std::endl;

	return ;
}

// ----
// _main

/*
int
main ( void ) // maybe renamed to webserv_init
{
	static struct timespec timeout = {1, 0};
	struct kevent ev = {};
	int fd, kq, n_events;

	(void)timeout;
	(void)fd;
	(void)ev;
	(void)kq;
	(void)n_events;
	
	// set SIGINT to gracefully stop the program
	::signal(SIGINT, &graceful_stop);

	std::set<const Server *> list;

	Server a(8080);
	Server b(8081);
	Server c(8082);

	list.insert(&a);
	list.insert(&b);
	list.insert(&c);

	kq = ::kqueue();
	if ( kq == -1 )
	{
		std::cerr << "Error creating kqueue" << std::endl;
		return (0x3);
	}
	else
		std::clog << "Created kqueue" << std::endl;

	std::set<const Server *>::const_iterator it = list.begin();
	while ( it != list.end() )
	{
		std::cout << (*it)->socket() << std::endl;
		EV_SET( &ev, (*it)->socket(), EVFILT_READ, EV_ADD | EV_ENABLE | EV_CLEAR, 0, 0, 0);
		if ( ::kevent( kq, &ev, 1, NULL, 0, &timeout) == -1 )
		{
			std::cerr << "kevent: " << strerror(errno) << std::endl;
			return (0x4);
		}
		++it;
	}

	while ( status == true )
	{
		// kevent will not return until at least one event is received
		// (or when an associated timeout is exhausted)
		n_events = ::kevent(kq, NULL, 0, &ev, 1, &timeout);

		std::clog << n_events << std::endl;
		
		if ( n_events == -1 )
		{
			std::cerr << "kevent: " << strerror(errno) << std::endl;
			return (0x5);
		}
		
		if ( n_events == 0 )
			continue ;
		
		std::clog << "id: " << ev.ident << std::endl;
	}

	return (0x0);
}
*/
