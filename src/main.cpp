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
	std::string   conf_filename;
	std::ifstream conf_file;

	// Check / set argument options

	decode_arguments( argc, argv );
	
	// If argv[optind] is set ( == NULL ),
	// falls back to using DEFAULT_CONF ( macro )
	
	conf_filename = (argv[optind] == 0x0 ? DEFAULT_CONF : argv[optind] );

	// isRegularFile() is in `parse.cpp', for the moment.
	// Check if `conf_filename' is a regular file.

	if ( isRegularFile( conf_filename ) == false )
	{
		std::cerr << PROGRAM_NAME;
		std::cerr << ": " << conf_filename;
		std::cerr << ": " << ::strerror( errno ) << std::endl;
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
	LOG( conf_filename << ": open OK" );

	// Add `conf_file' is contents.
	// Server_conf::add() also parses them. 
	
		//if ( ServerConf::add( conf_file ) == EXIT_FAILURE )
		//{
		//	conf_file.close();
		//	return ( EXIT_FAILURE );
		//}
	
	// Once done, close file;

	conf_file.close();
	LOG( conf_filename << ": closed OK" );

		// DEBUG
		ServerConf * serv;
		serv = new ServerConf();
		serv->_address.sin_port = htons( 9000 );
		serv->_address.sin_family = AF_INET;
		serv->_address.sin_addr.s_addr = htonl( INADDR_ANY );
		serv->_root = std::string( "/Users/mpuig-ma/Documents/www" );
		serv->_allow_methods.push_back( std::string( "GET") );
		serv->_client_max_body_size = 0;
		serv->_index = std::string();
		serv->_cgi_param = std::string( "params" );
		serv->_cgi_pass = std::string( "pass" );
		serv->_autoindex = true;
		ServerConf::instances.push_back( serv );
		serv = new ServerConf();
		serv->_address.sin_port = htons( 8000 );
		serv->_address.sin_family = AF_INET;
		serv->_address.sin_addr.s_addr = htonl( INADDR_ANY );
		serv->_root = std::string( "/Users/mpuig-ma/Documents" );
		serv->_allow_methods.push_back( std::string( "GET") );
		serv->_client_max_body_size = 0;
		serv->_index = std::string();
		serv->_cgi_param = std::string( "params" );
		serv->_cgi_pass = std::string( "pass" );
		serv->_autoindex = true;
		ServerConf::instances.push_back( serv );

	// Initialize n Server instances based on the configuration file.
	// new `Server's in Server::servers will have to be FREEd.

	for ( ServerConf::iterator it = ServerConf::instances.begin();
			it != ServerConf::instances.end(); ++it )
	{
		Server::servers.push_back( new Server( *( *it ) ) );
		if ( ! Server::servers.back()->good )
		{
			Server::clear();
			return ( EXIT_FAILURE );
		}

	}

	ServerConf::clear();
	Server::servers.shrink_to_fit();

	// Start program's purpose.

	if ( ::webserv() != EXIT_SUCCESS )
	{
		std::cerr << PROGRAM_NAME;
		std::cerr << ": hmm ??";
		std::cerr << std::endl;
		return ( EXIT_FAILURE );
	}

	return ( EXIT_SUCCESS );
}
