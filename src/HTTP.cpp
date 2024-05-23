/*                                                                            */
/* HTTP.cpp                                                                   */
/* mpuig-ma <mpuig-ma@student.42barcelona.com>                                */
/* Tue Apr  9 16:50:09 2024                                                   */

#include "HTTP.hpp"

void clear_request ( t_request & );
void clear_response ( t_response & );

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
	_state( 0 ),
	_expect( false )
{
	clear_request( this->_request );
	clear_response( this->_response );
	this->_server = this->_router.getDefaultServer();
	this->_socket_fd = ::accept( fd,
			(struct sockaddr *) &this->_address, &this->_address_len );
	if ( this->_socket_fd == -1
			|| fcntl( this->_socket_fd, F_SETFL, O_NONBLOCK, FD_CLOEXEC ) == -1 )
	{
		ERROR( "HTTP: error creating new client" );
		delete this;
		return ;
	}
	this->register_recv();
	return ;
}

HTTP::~HTTP ( void )
{
	LOG("Destructor: " << this->_socket_fd);
	(void) close( this->_socket_fd );
	this->deregister_recv();
	this->deregister_timer();
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
	return ;
}

void
HTTP::setStatusCode ( int value )
{
	this->_response.status_code = value;
	return ;
}

void
HTTP::setState ( int value )
{
	this->_state = value;
	return ;
}

void
clear_request ( t_request & request )
{
	request.http_version = 0;
	request.method = NULL;
	request.host.clear();
	request.target.clear();
	request.query.clear();
	request.body.clear();
	request.file.clear();
	std::memset( &request.file_info, 0, sizeof( request.file_info ) );
	request.headers.clear();
	return ;
}

void
clear_response ( t_response & response )
{
	response.status_code = 0;
	response.body.clear();
	response.headers.clear();
	return ;
}

int
HTTP::getState ( void )
{
	return this->_state;
}
