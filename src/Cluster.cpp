#include "Cluster.hpp"

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