/*                                                                            */
/* ServerConf.cpp                                                             */
/* mpuig-ma <mpuig-ma@student.42barcelona.com>                                */
/* Wed Apr  3 11:31:15 2024                                                   */

#include "ServerConf.hpp"

ServerConf::ServerConf ( void ):
	_flags ( 0 )
{
	return ;
}

ServerConf::~ServerConf ( void )
{
	return ;
}

bool
ServerConf::getFlag ( int mask ) const
{
	if ( this->_flags & mask )
		return ( true );
	return ( false );
}

std::size_t
ServerConf::getFlags ( void ) const
{
	return ( this->_flags );
}

std::size_t
ServerConf::getClientMaxBodySize ( void ) const
{
	return ( this->_client_max_body_size );
}

std::string
ServerConf::getCGIparam ( void ) const
{
	return ( this->_cgi_param );
}

std::string
ServerConf::getCGIpass ( void ) const
{
	return ( this->_cgi_pass );
}

std::string
ServerConf::getRoot ( void ) const
{
	return ( this->_root );
}

std::vector< std::string >
ServerConf::getServerNames ( void ) const
{
	return ( this->_server_name );
}

std::vector< std::string >
ServerConf::getIndex ( void ) const
{
	return ( this->_index );
}

std::string
ServerConf::getErrorPage ( int errnum )
{
	std::string page;

	if ( this->_error_page.find( errnum ) != this->_error_page.end() )
		page.assign( this->_error_page[errnum] );
	return ( page );
}

int
ServerConf::setFlag ( int flag, bool enable )
{
	//DEBUG( std::boolalpha << enable << " " << std::hex << flag << std::dec );
	if ( enable == true )
		this->_flags |= flag;
	else
		this->_flags ^= flag;
	return ( EXIT_SUCCESS );
}

int
ServerConf::setClientMaxBodySize ( std::size_t size )
{
	//DEBUG( std::dec << size );
	this->_client_max_body_size = size;
	return ( EXIT_SUCCESS );
}

int
ServerConf::setCGIparam ( std::string & arg )
{
	//TODO: check if multiple params ( which should be invalid )
	//TODO: check arg is valid
	//return ( EXIT_FAILURE );
	//DEBUG( arg.c_str() );
	this->_cgi_param = arg;
	return ( EXIT_SUCCESS );
}

int
ServerConf::setCGIpass ( std::string & arg )
{
	//TODO: check arg is valid
	//return ( EXIT_FAILURE );
	//DEBUG( arg.c_str() );
	this->_cgi_pass = arg;
	return ( EXIT_SUCCESS );
}

int
ServerConf::setRoot ( std::string & arg )
{
	//TODO: check arg is valid
	//return ( EXIT_FAILURE );
	//DEBUG( arg.c_str() );
	this->_root = arg;
	while ( this->_root.back() == '/' )
		this->_root.erase( this->_root.length() - 1, 1 );
	return ( EXIT_SUCCESS );
}

int
ServerConf::setServerName ( std::string & arg )
{
	//TODO: check values
	//return ( EXIT_FAILURE );
	//DEBUG( arg.c_str() );
	if ( arg.empty() )
		return ( EXIT_FAILURE );
	if ( ! isalpha( arg[0] ) )
		return ( EXIT_FAILURE );
	for ( std::string::size_type i = 0; i < arg.length(); i++ )
	{
		if ( ! isalnum( arg[i] ) && arg[i] != '-' && arg[i] != '.' )
          	return ( EXIT_FAILURE );
	}
    this->_server_name.push_back( arg );
	return ( EXIT_SUCCESS );
}

int
ServerConf::setIndex ( std::string & arg )
{
	//TODO: check value
	//return ( EXIT_FAILURE );
	//DEBUG( arg.c_str() );
	if ( std::find( this->_index.begin(),
				this->_index.end(), arg ) == this->_index.end() )
		this->_index.push_back( arg );
	return ( EXIT_SUCCESS );
}

int
ServerConf::setErrorPage ( int n, std::string & path )
{
	//TODO: check values
	//return ( EXIT_FAILURE );
	//DEBUG( n << " " << path );
	if ( n < 400 || n > 555 )
		return ( EXIT_FAILURE );
	this->_error_page[n] = path;
	return ( EXIT_SUCCESS );
}

void
ServerConf::log_conf ( void )
{
	LOG( "root=" << this->getRoot() );
	LOG( "autoindex=" << std::boolalpha << this->getFlag( F_AUTOINDEX ) );
	LOG( "GET=" << std::boolalpha << this->getFlag( METHOD_GET ) );
	LOG( "POST=" << std::boolalpha << this->getFlag( METHOD_POST ) );
	LOG( "PUT=" << std::boolalpha << this->getFlag( METHOD_PUT ) );
	LOG( "DELETE=" << std::boolalpha << this->getFlag( METHOD_DELETE ) );
	LOG( "HEAD=" << std::boolalpha << this->getFlag( METHOD_HEAD ) );
	LOG( "cgi_param=" << this->_cgi_param );
	LOG( "cgi_pass=" << this->_cgi_pass );
	for ( std::vector< std::string >::iterator it = this->_server_name.begin();
			it != this->_server_name.end(); ++it )
		LOG( "server_name=" << *it );
	for ( std::vector< std::string >::iterator it = this->_index.begin();
			it != this->_index.end(); ++it )
		LOG( "index=" << *it );
	return ;
}
