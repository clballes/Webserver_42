/*                                                                            */
/* Location.cpp                                                               */
/* mpuig-ma <mpuig-ma@student.42barcelona.com>                                */
/* Wed Apr 17 17:14:14 2024                                                   */

#include "Location.hpp"
#include "file.hpp"

Location::Location ( void ): _flags( 0 ), _isDefault( false )
{
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
	return ;
}

bool
Location::getFlag ( int mask ) const
{
	return ( this->_flags & mask );
}

std::size_t
Location::getFlags ( void ) const
{
	return ( this->_flags );
}

const std::string &
Location::getCGIparam ( void ) const
{
	return ( this->_cgi_param );
}

const std::string &
Location::getCGIpass ( void ) const
{
	return ( this->_cgi_pass );
}

const std::string &
Location::getUploadfile ( void ) const
{
	return ( this->_upload_files );
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

std::vector< std::string > &
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
	return ( EXIT_SUCCESS );
}

int
Location::setCGIparam ( std::string & arg )
{
	// TODO: validate
	this->_cgi_param.assign( arg );
	return ( EXIT_SUCCESS );
}

int
Location::setCGIpass ( std::string & arg )
{
	// TODO: validate
	this->_cgi_pass.assign( arg );
	return ( EXIT_SUCCESS );
}

int
Location::setRoot ( std::string & arg )
{
	// TODO: validate
	this->_root.assign( arg );
	return ( EXIT_SUCCESS );
}

int
Location::setIndex ( std::string & arg )
{
	// TODO: validate
	this->_index.push_back( arg );
	return ( EXIT_SUCCESS );
}

int
Location::setUploadFiles ( std::string & arg )
{
	if ( can_access_file(arg ) == false)
	{
		return ( EXIT_FAILURE );
	}
	// TODO: validate
	this->_upload_files = arg;
	return ( EXIT_SUCCESS );
}

int
Location::setRedirection ( std::string & arg )
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

void
Location::log_conf ( void ) const
{
	LOG( BLUE << "root=" << this->getRoot() );
	LOG( BLUE << "isDefault=" << this->isDefault() );
	LOG( BLUE << "flags=" << std::hex << this->getFlags() << std::dec );
	LOG( BLUE << "cgi_pass=" << this->getCGIpass() );
	LOG( BLUE << "cgi_param=" << this->getCGIparam() );
	return ;
}
