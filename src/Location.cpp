/*                                                                            */
/* Location.cpp                                                               */
/* mpuig-ma <mpuig-ma@student.42barcelona.com>                                */
/* Wed Apr 17 17:14:14 2024                                                   */

#include "Location.hpp"
#include "file.hpp"
#include <unistd.h>

Location::Location ( void ): _isDefault( false )
{
	this->_flags = 0x11111111;
	this->setFlag( F_AUTOINDEX, false );
	this->_client_max_body_size = 1 * 1024 * 1204;
	return ;
}

Location::~Location ( void )
{
	return ;
}

void
Location::setDefault ( void )
{
	this->_isDefault = true;
	this->_flags = 0x11111111;
	// in nginx the default value for client_max is 1MB, then 1,048,576bytes
	this->_client_max_body_size = 1 * 1024 * 1204;
	return ;
}

bool
Location::getFlag ( int mask ) const
{
	return ( ( this->_flags & mask ) ? true : false );
}

std::size_t
Location::getFlags ( void ) const
{
	return ( this->_flags );
}

// const std::string &
// Location::getCGIparam ( void ) const
// {
// 	return ( this->_cgi_param );
// }

const std::string &
Location::getCGIpass ( void ) const
{
	return ( this->_cgi_pass );
}

const std::size_t &
Location::getClientMaxBodySize ( void ) const
{
	return ( this->_client_max_body_size );
}

const std::pair<int, std::string >&
Location::getRedirection ( void ) const
{
	return ( this->_redirection );
}

const std::string &
Location::getRoot ( void ) const
{
	return ( this->_root );
}

const std::vector< std::string > &
Location::getIndex ( void ) const
{
	return ( const_cast< std::vector< std::string > & >( this->_index ) );
}

bool
Location::isDefault ( void ) const
{
	return ( this->_isDefault );
}

int
Location::setFlag ( int flag, bool enable )
{
	if ( enable == true )
		this->_flags |= flag;
	else
		this->_flags ^= flag;
	return ( EXIT_SUCCESS );
}

// int
// Location::setCGIparam ( const std::string & arg )
// {
// 	// TODO: validate
// 	this->_cgi_param.assign( arg );
// 	return ( EXIT_SUCCESS );
// }

int
Location::setCGIpass ( const std::string & arg )
{
	// TODO: validate
	this->_cgi_pass.assign( arg );
	return ( EXIT_SUCCESS );
}

int
Location::setClientMaxBodySize ( const std::string & arg )
{
	std::size_t n = 0;
    char alpha = 'B'; // Default unit is bytes
    if (arg.empty() || arg.find(" ") != std::string::npos)
    {
        ERROR("invalid number of arguments in \"client_max_body_size\"");
        return EXIT_FAILURE;
    }
    for (std::size_t i = 0; i < arg.length(); i++)
    {
        if (!isdigit(arg[i]) && arg[i] != 'm' && arg[i] != 'M' && arg[i] != 'k' && arg[i] != 'K')
        {
            ERROR("directive client_max_body_size \"" << arg << "\" wrong format");
            return EXIT_FAILURE;
        }

        if (isalpha(arg[i])) // Check if character is a letter
        {
            alpha = arg[i]; // Store the letter
        }

        if (alpha != 'B' && i != arg.length() - 1) // If a unit is provided, ensure it's the last character
        {
            ERROR("directive client_max_body_size \"" << arg << "\" wrong format");
            return EXIT_FAILURE;
        }
    }

    n = static_cast<std::size_t>(std::atoi(arg.c_str()));

    // Convert size to bytes if unit is provided
    if (alpha == 'M' || alpha == 'm')
    {
        n *= 1024 * 1024; // Convert megabytes to bytes
    }
    else if (alpha == 'K' || alpha == 'k')
    {
        n *= 1024; // Convert kilobytes to bytes
    }

    this->_client_max_body_size = n;
	return (EXIT_SUCCESS);
}


int
Location::setRoot ( const std::string & arg )
{
	// TODO: validate
	std::string mod_arg( arg );

	while ( mod_arg.size() > 1 && mod_arg.back() == '/' )
		mod_arg.erase( mod_arg.length() - 1, 1 );
	this->_root.assign( mod_arg );
	return ( EXIT_SUCCESS );
}

int
Location::setIndex ( const std::string & arg )
{
	// TODO: validate
	this->_index.push_back( arg );
	return ( EXIT_SUCCESS );
}

int
Location::setRedirection ( const std::string & arg )
{
	std::size_t spacePos = arg.find(' ');

    std::string statusCodeStr = arg.substr(0, spacePos);
    int statusCode;
    std::istringstream(statusCodeStr) >> statusCode;
	if (statusCode != 301 && statusCode != 302 && statusCode != 308 && statusCode != 303 && statusCode != 307 )
	{
		return (EXIT_FAILURE);
	}
	//nose si em faltaria parsejar si la ruta existeix de la direccio o ja petara despres
    // Extract the route
    std::string route = arg.substr(spacePos + 1);
	this->_redirection = std::make_pair(statusCode, route);
	return ( EXIT_SUCCESS );
}
