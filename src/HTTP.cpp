/*                                                                            */
/* HTTP.cpp                                                                   */
/* mpuig-ma <mpuig-ma@student.42barcelona.com>                                */
/* Tue Apr  9 16:50:09 2024                                                   */

#include "HTTP.hpp"

t_http_method
HTTP::methods[] = {
	{ "GET", &HTTP::http_get, HTTP_GET },
	{ "HEAD", &HTTP::http_head, HTTP_HEAD },
	{ "POST", &HTTP::http_post, HTTP_POST },
	{ "PUT", &HTTP::http_put, HTTP_PUT },
	{ "DELETE", &HTTP::http_delete, HTTP_DELETE },
	{ 0, 0, 0 }
};

HTTP::HTTP ( Router & router_instance, int fd ):
	_socket_fd( 0 ),
	_router( router_instance ),
	_connection( router_instance.getConnection( fd ) ),
	_server( router_instance.getDefaultServer() ),
	_cgi_ptr( NULL ),
	_status_code( 0 ),
	_expect( false )
{
	this->_request.method = 0x0;
	std::memset( &this->_request.file_info, 0,
			sizeof( this->_request.file_info ) );
	this->_request.http_version = 0;
	this->_socket_fd = ::accept( fd,
			(struct sockaddr *) &this->_address, &this->_address_len );
	if ( this->_socket_fd == -1
			|| fcntl( this->_socket_fd, F_SETFL, O_NONBLOCK, FD_CLOEXEC ) == -1 )
	{
		ERROR( "HTTP: error creating new client" );
		return ;
	}
	this->register_recv();
	return ;
}

HTTP::~HTTP ( void )
{
	(void) close( this->_socket_fd );
	return ;
}

// TODO: keep list of clients in order to delete them if Control+C

Server &
HTTP::getServer ( void )
{
	return ( this->_server );
}

t_request &
HTTP::getRequest ( void )
{
	return ( this->_request );
}

t_response &
HTTP::getResponse ( void )
{
	return ( this->_response );
}

t_headers &
HTTP::getRequestHeaders ( void )
{
	return ( this->_request.headers );
}

t_headers &
HTTP::getResponseHeaders ( void )
{
	return ( this->_response.headers );
}

void
HTTP::setMessageBody ( const std::string & message )
{
	this->_response.body.append( message );
	return ;
}

void
HTTP::setResponseHeaders ( const std::string & name,
		const std::string & value )
{
	this->_response.headers[name] = value;
	//TODO: what is this for ???
	//if ( name == "status" )
	//	this->setStatusCode( std::atoi( value.c_str() ) );
	return ;
}

void
HTTP::setStatusCode ( int value )
{
	this->_status_code = value;
	return ;
}
