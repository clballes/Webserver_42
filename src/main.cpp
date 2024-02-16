/*                                                                            */
/* main.cpp                                                                   */
/* mpuig-ma <mpuig-ma@student.42barcelona.com>                                */
/* Thu Jan 25 14:26:11 2024                                                   */

#include "webserv.hpp"
#include "init.hpp"
#include "parse.hpp"
#include <fstream>

int
main ( int argc, char * const * argv )
{
	// Check / set argument options

	decode_arguments( argc, argv );

	std::string conf_filename;
	std::ifstream conf_file;
	
	// If argv[optind] is set ( == NULL ),
	// falls back to using DEFAULT_CONF ( macro )
	
	conf_filename = (argv[optind] == 0x0 ? DEFAULT_CONF : argv[optind] );

	// isRegularFile() is in `parse.cpp', for the moment.
	// Check if `conf_filename' is a regular file.

	if ( isRegularFile( conf_filename ) == false )
	{
		std::cerr << PROGRAM_NAME;
		std::cerr << ": " << conf_filename;
		std::cerr << ": not a file" << std::endl;
		return ( EXIT_FAILURE );
	}
	
	// Open ( or try to ) `conf_file'.
	
	conf_file.open( conf_filename );

	// Check if file has been properly opened.

	if ( conf_file.good() != true )
	{
		std::cerr << ( argv[optind] == 0x0 ? DEFAULT_CONF : argv[optind] );
		std::cerr << ": " << ::strerror( errno ) << std::endl;
		return ( EXIT_FAILURE );
	}
	LOG( conf_filename << ": open OK" )

	// Add `conf_file' is contents.
	// Server_conf::add() also parses them. 
	
	if ( ServerConf::add( conf_file ) == EXIT_FAILURE )
	{
		conf_file.close();
		return ( EXIT_FAILURE );
	}
	
	// Once done, close file;

	conf_file.close();
	LOG( conf_filename << ": closed" )

	// --
	// This goes to another function 

	// Initialize n Server instances based on the configuration file.

	ServerConf::iterator it = ServerConf::instances.begin();

	while ( it != ServerConf::instances.end() )
	{
		Server::servers.push_back( new Server( *(*it) ) );
		++it;
	}

	// Return extra memory to the system as <vector> is being used.

	//Server::servers.shrink_to_fit();
	//better to use reserve() previously
	
		// For the purpose of testing;
		// initialize some servers manually.

		//Server a( 8080 ); Server::servers.push_back( &a );
		//Server b( 80 ); Server::servers.push_back( &b );
		//Server c( USHRT_MAX ); Server::servers.push_back( &c );
		//Server d( 22 ); Server::servers.push_back( &d );
		//Server e( 80 ); Server::servers.push_back( &e );
		//Server f( 00 ); Server::servers.push_back( &f );
		
		// --

	// -- 

	// Start listening / accepting connections
	// ... kqueue
	
	//::webserv( Server::servers );
	::webserv();

	return ( EXIT_SUCCESS );
}
