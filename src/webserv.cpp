/*                                                                            */
/* test-kqueue.cpp                                                            */
/* mpuig-ma <mpuig-ma@student.42barcelona.com>                                */
/* Thu Jan 25 14:26:11 2024                                                   */

#include "webserv.hpp"
#include <fstream>
#include <set>
#define LOG(s) { std::clog << s << std::endl; }

int decode_arguments ( int, char * const * );

int
main ( int argc, char * const * argv )
{
	decode_arguments( argc, argv );

	return (EXIT_SUCCESS);
}

/*
void graceful_stop (int);

bool status = true;

void
graceful_stop (int n)
{
	(void) n;
	std::clog << "\rGracefully stoppping..." << std::endl;
	sleep(1);
	status = false;
}
*/

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

// ----
// OLD MAIN

/*
int
main (int argc, const char **argv)
{
	std::string		conf_file;

	#ifdef SILENCE_LOGS
	std::ofstream log_file;
	log_file.open("logs.txt", std::ostream::app);
	std::clog.rdbuf(log_file.rdbuf());
	#endif

	std::clog << "webserv" << std::endl;

	conf_file.assign(DEFAULT_CONF);
    if (argc > 2)
    {
		std::cout << argv[0] << " [configuration file]";
		std::cout << std::endl;
		return (EXIT_SUCCESS);
    }

	if (argv[1] != NULL)
		conf_file.assign(argv[1]);

	if (conf_file.compare("-v") == 0
		|| conf_file.compare("--version") == 0)
	{
		std::cout << WEBSERV_VER << std::endl;
		return (EXIT_SUCCESS);
	}

	Server* server_instance = new Server;
	server_instance->setName("puyma.xyz");
	server_instance->setPort(8000);
	server_instance->setAddr(INADDR_ANY);
	server_instance->setAddr(0x1C1C1C1C);

	std::cout << *server_instance << std::endl;

	if (server_instance->ok() == WEBSERV_OK)
		server_instance->start();

	delete server_instance;
	return (EXIT_SUCCESS);
}
*/
