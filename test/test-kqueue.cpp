/*                                                                            */
/* test-kqueue.cpp                                                            */
/* mpuig-ma <mpuig-ma@student.42barcelona.com>                                */
/* Thu Jan 25 14:26:11 2024                                                   */

#include "webserv.hpp"
#define LOG(s) { std::clog << s << std::endl; }

// https://wiki.netbsd.org/examples/socket_programming/
// https://wiki.netbsd.org/tutorials/kqueue_tutorial/

int do_socket_stuff (int);
void graceful_stop (int);

bool status = true;

int
do_socket_stuff (int port)
{
	int socket_file_descriptor;

	(void) port;
	socket_file_descriptor = ::socket(AF_INET, SOCK_STREAM, 0x6);

	return (socket_file_descriptor);
}

void
graceful_stop (int n)
{
	(void) n;
	std::clog << "\rGracefully stoppping..." << std::endl;
	sleep(1);
	status = false;
}

// ----
// Class Server

class Server
{
	public:

		Server ( unsigned );
		~Server ( void );

		unsigned socket (void) const;

	private:

		int _socket_fd;
		struct sockaddr_in _server_address;
		int _client_socket_fd;
		unsigned int _client_address_len;
		struct sockaddr_in _client_address; // ::accept() can only do one client at a time ??

};

Server::Server ( unsigned port )
{
	::memset ( &this->_socket_fd, 0, sizeof( this->_socket_fd ) );
	this->_socket_fd = ::socket( AF_INET, SOCK_STREAM, IPPROTO_TCP );

	::memset( &this->_server_address, 0, sizeof( this->_server_address ) );
	this->_server_address.sin_family = AF_INET;
	this->_server_address.sin_addr.s_addr = htonl( INADDR_ANY );
	this->_server_address.sin_port = htons( port );

	LOG ( this << ": port " << port )
	LOG ( this << ": socket " << this->_socket_fd )
	
	if ( ::bind( this->_socket_fd, (struct sockaddr *) &this->_server_address, sizeof( this->_server_address ) ) < 0 )
	{
		std::cerr << this << ": bind " << ::strerror( errno ) << std::endl;
		::exit (0x1);
	}

	LOG ( this << ": bind OK" )

	if ( ::listen( this->_socket_fd, 0x0 ) < 0 )
	{
		std::cerr << this << ": listen " << ::strerror( errno ) << std::endl;
		::exit (0x2);
	}	
	
	LOG ( this << ": listening" )
	
	// just listen
	// put event on kqueue and accept when there's activity
	//_client_address_len = sizeof( this->_client_address );
	//_client_socket_fd = ::accept( this->_socket_fd, (struct sockaddr *) &this->_client_address, &this->_client_address_len );

	::send( this->_client_socket_fd, "Hey there\n", ::strlen( "Hey there\n" ), 0 );

	(void) _client_address_len;
	(void) _client_socket_fd;
	(void) _client_address;

	return ;
}

Server::~Server (void)
{
	if (this->_socket_fd != 0)
		close (this->_socket_fd);

	return ;
}

unsigned
Server::socket (void) const
{
	return (this->_socket_fd);
}

#include <set>

// ----
// _main

int
main ( void )
{
	static struct timespec timeout = {1, 0};
	struct kevent ev = {};
	int fd, kq, n_events;

	(void)timeout;
	(void)fd;
	(void)ev;
	(void)kq;
	(void)n_events;
	
	std::set<const Server *> list;

	Server a(8080);
	Server b(8081);
	Server c(8082);

	list.insert(&a);
	list.insert(&b);
	list.insert(&c);

	// set SIGINT to gracefully stop the program
	::signal(SIGINT, &graceful_stop);

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

		if ( ev.fflags & NOTE_WRITE )
		{
			std::clog << "written" << std::endl;
			ev.fflags &= ~NOTE_WRITE;
			exit(9);
		}
		else
		{
			std::clog << "unknown event" << std::endl;
		}
	}

	return (0x0);
}
