/*                                                                            */
/* Controller.hpp                                                             */
/* mpuig-ma <mpuig-ma@student.42barcelona.com>                                */
/* Mon Mar 25 11:51:10 2024                                                   */

#ifndef _CONTROLLER_HPP_
#define _CONTROLLER_HPP_

#include <string>
#include <vector>
#include <iostream>
#include <fstream>

#include "define.hpp"
#include "debug.hpp"

bool is_regular_file( const std::string & );

// TODO: Make Controller a singleton ?
// TODO: expand template to define underlying container; default vector

/*
 * class Controller< T>:
 *
 * Provides
 *  - a way to load multiple configuration files
 *  - create and configure servers ( from the configuration )
 *  - handles a loop queue 
 *
 */

template<typename T>
class Controller 
{
	public:

		Controller ( void );
		~Controller ( void );

		bool good ( void ) const;
		int load ( const char * );
		int load ( std::string & );
		int start ( void );

		const std::vector< T > & getInstances ( void ) const;

		typedef typename std::vector< T >::iterator iterator;

	private:

		bool _good;
		std::vector< T > _instances;

};

template<typename T>
Controller< T >::Controller ( void ): _good( true )
{
	DEBUG( this );

	return ;
}

template<typename T>
Controller< T >::~Controller ( void )
{
	DEBUG( this );

	return ;
}

template<typename T>
bool
Controller< T>::good ( void ) const
{
	return ( this->_good );
}

template<typename T>
int
Controller< T >::load ( const char * conf_filename_ptr )
{
	std::ifstream conf_file;
	
	DEBUG( this );

	if ( conf_filename_ptr == 0x0 )
	{
		std::clog << "[" << this << "] ";
		std::clog << "0x0: cannot load" << std::endl;
		return ( EXIT_FAILURE );
	}

	DEBUG( conf_filename_ptr );
	this->_good = false;

	// Check if `conf_filename' is a regular file.
	if ( is_regular_file( conf_filename_ptr ) == false )
	{
		// TODO: errno might not be set.
		ERROR( conf_filename_ptr << ": " << ::strerror( errno ) );
		this->_good = false;
		return ( EXIT_FAILURE );
	}

	// Open ( or try to ) `conf_file'.
	conf_file.open( conf_filename_ptr );
	
	// Check if file has been properly opened.
	if ( conf_file.good() != true )
	{
		ERROR( conf_filename_ptr << ": " << ::strerror( errno ) );
		this->_good = false;
		return ( EXIT_FAILURE );
	}
	
	// file2mem
	// parse
	// WIP

	/*
	// Server_conf::add() also parses them. 
	if ( ServerConf::add( conf_file ) == EXIT_FAILURE )
	{
		conf_file.close();
		return ( EXIT_FAILURE );
	}

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
	*/

	// Once done, close file;
	conf_file.close();

	return ( EXIT_SUCCESS );
}

template<typename T>
int
Controller< T >::load ( std::string & conf_filename )
{	
	DEBUG( this );

	return ( this->load( conf_filename.c_str() ) );
}

template<typename T>
int
Controller< T >::start ( void )
{
	DEBUG( this );

	if ( this->good() == false )
	{
		ERROR( "webserv: faulty controller" );
		return ( EXIT_FAILURE );
	}

	this->_instances.shrink_to_fit();

	for ( Controller::iterator it = this->_instances.begin();
			it != this->_instances.end(); ++it )
	{
		if ( it->good() == false )
		{
			ERROR( PROGRAM_NAME << ": faulty server (" << it->id() << ")" );
			return ( EXIT_FAILURE );
		}
		if ( it->start() )
		{
			// TODO: retry once after ~30 seconds.
			return ( EXIT_FAILURE );
		}
	}

	return ( EXIT_SUCCESS );
}

template<typename T>
const std::vector< T > &
Controller< T>::getInstances ( void ) const
{
	DEBUG( this );
	
	INFO( "capacity: " << this->_instances.capacity() );

	return ( this->_instances );
}

#include <sys/stat.h>

bool
is_regular_file( const std::string & filename )
{
	struct stat file_info;
	
	DEBUG( filename.c_str() );

    if ( stat( filename.c_str(), &file_info ) != 0 )
        return ( false );

    return ( S_ISREG( file_info.st_mode ) );
}

#endif /* !_CONTROLLER_HPP_ */
