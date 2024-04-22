/*                                                                            */
/* Location.cpp                                                               */
/* mpuig-ma <mpuig-ma@student.42barcelona.com>                                */
/* Wed Apr 17 17:14:14 2024                                                   */

#include "Location.hpp"

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
	// TODO: validate
	this->_upload_files = arg;
	return ( EXIT_SUCCESS );
}

int
Location::setRedirection ( std::string & arg )
{
	// TODO: validate
	this->_redirection = arg;
	return ( EXIT_SUCCESS );
}
