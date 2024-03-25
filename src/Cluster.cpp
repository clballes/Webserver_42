#include "Cluster.hpp"

void event_loop ( int );
int
IEvent::kq = 0;
std::vector<Server> Cluster::_server;
bool Cluster::_status = true;

Cluster::Cluster(void)
{
    std::cout << "Constructor called" << std::endl;
}

// Cluster::Cluster(const Cluster & src)
// {
//     std::cout << "Copy constructor called" << std::endl;

// }
Cluster::~Cluster(void)
{
    std::cout << "Destructor cluster called "<< std::endl;

}
int	Cluster::config(std::string conf_filename )
{
    std::ifstream conf_file;
    
    // is_regular_file() is in `parse.cpp', for the moment.
	// Check if `conf_filename' is a regular file.

    if ( is_regular_file( conf_filename ) == false )
	{
		std::cerr << PROGRAM_NAME;
		std::cerr << ": " << conf_filename;
		std::cerr << ": " << ::strerror( errno ) << std::endl;
		return ( EXIT_FAILURE );
	}

    conf_file.open( conf_filename );

	LOG( conf_filename << ": open OK" );

    // Add `conf_file' is contents.
	// Server_conf::add() also parses them. 
	
	if ( _conf.add( conf_file ) == EXIT_FAILURE )
	{
		conf_file.close();
		return ( EXIT_FAILURE );
	}

	// Once done, close file;
	conf_file.close();
	LOG( conf_filename << ": closed OK" );
    return (EXIT_SUCCESS);
}

// function to setup minimum info on server,
// server should have the adress listen and port
int Cluster::setup(void)
{
	std::cout << "SETUP CLUSTER:" << _conf.getServers().size() << std::endl;
	const std::vector<ServerConf>& serverConfs = _conf.getServers();
	for(size_t i = 0; i < serverConfs.size(); i++)
    {
        Server server(serverConfs[i].getAddress());
		this->_server.push_back( server );
	}
	return (EXIT_SUCCESS);
}

void
Cluster::graceful_stop ( int n )
{
	LOG( "\nStopping " << PROGRAM_NAME << " gracefully (s=" << n << ")" );
	// _status = false;

	for (std::vector<Server>::iterator it = _server.begin();
		it != _server.end(); ++it )
		( *it ).stop();

	close( IEvent::kq );

	return ;
}

void
Cluster::event_loop ( int kq )
{
	int				n_events;
	struct kevent	ev;
	IEvent *        instance;

	n_events = 1;
	_status = true;

	LOG( "call event_loop() (fd=" << kq << ")" );

	while ( _status == true )
	{
		// kevent() call does not return until at least one event is received
		// or when an associated timeout is exhausted.

		n_events = ::kevent( kq, 0x0, 0, &ev, 1, 0 );

		if ( _status == false )
			return ;

		if ( n_events == -1 )
		{
			std::cerr << "kevent: " << ::strerror( errno ) << std::endl;
			break ; // ( EXIT_FAILURE );
		}
		
		if ( n_events == 0 )
			continue ;

		LOG( "(ev=" << ev.ident << ")" );
		
		if ( ev.flags & EVFILT_READ )
		{
			instance = static_cast< IEvent * >( ev.udata );
			instance->dispatch( ev );
		}

		// consider EVFILT_SIGNAL
	}

	LOG( "close kq (fd=" << IEvent::kq << ")" );
	close ( IEvent::kq );

	return ;
}

int Cluster::webserver(void)
{
	LOG( "call webserv()" );

	// Configure SIGINT ( signal interrupt )
	// so as to finish connections and end the program
	// gracefully.
	
	::signal( SIGINT, Cluster::graceful_stop );
	::signal( SIGQUIT, Cluster::graceful_stop );

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

	// TODO: maybe just set_init_events (sockets mainly)
	// ; there will be more events to register later,
	// during program execution and loop execution.

	for (std::vector<Server>::iterator it = _server.begin();
			it != _server.end(); ++it )
	{
		if ( ( *it ).start() )
		{
			// TODO: retry once after ~30 seconds.
			return ( EXIT_FAILURE );
		}
	}

	// Start listening for registered events
	// ... kqueue

	event_loop( IEvent::kq );

	return ( EXIT_SUCCESS );
}

