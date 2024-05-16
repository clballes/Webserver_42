//                                                                             /
// HTTP_request.cpp                                                            /
// mpuig-ma <mpuig-ma@student.42barcelona.com>                                 /
// Thu May 16 12:31:49 2024                                                    /

#include "HTTP.hpp"

#define BUFFER_SIZE 512

int
HTTP::recv_request ( int64_t data )
{
	ssize_t			n;
	std::string		buffer;

	buffer.resize( data );
	n = recv( this->_socket_fd, (char *) buffer.data(), data, 0 );
	if ( n == -1 )
	{
		ERROR( std::strerror( errno ) );
		return ( EXIT_FAILURE );
	}
	else if ( n == 0 )
	{
		INFO( "Client closed connection" );
		delete this;
		return ( EXIT_SUCCESS );	
	}
	this->_buffer_recv.append( buffer );
	return ( EXIT_SUCCESS );
}

/*
int
HTTP::recv_request ( int64_t data )
{
	ssize_t		n;
	bool		cont;
	char		tmp_buffer[BUFFER_SIZE];

	// Receive content from socket.
	(void) data;
	cont = true;
	n = 1;
	while ( n > 0 && cont == true )
	{
		n = recv( this->_socket_fd, tmp_buffer, BUFFER_SIZE, 0 );
		this->_buffer_recv.append( tmp_buffer, 0, BUFFER_SIZE );
		if ( this->_buffer_recv.find( "\r\n\r\n" ) != std::string::npos )
			cont = false;
	}
	if ( n == -1 )
	{
		INFO( std::strerror( errno ) );
		delete this;
		return ( EXIT_FAILURE );
	}
	else if ( n == 0 )
	{
		INFO( "Client closed connection (fd=" << this->_socket_fd << ")" );
		delete this;
		return ( EXIT_SUCCESS );
	}

	// If request sent "expect: 100-continue" header
	if ( this->_expect == true )
	{
		this->_request.body.append( this->_buffer_recv );
		// TODO: will need to check if end expect
		this->_expect = false;
		this->_request.headers.erase( "expect" );
		if ( this->_request.headers.find( "transfer-encoding" ) != this->_request.headers.end()
				&& this->_request.headers.at( "transfer-encoding" ) == "chunked" )
		{
			if ( unchunk( this->_request.body, this->_response.body ) == EXIT_FAILURE )
			{
				ERROR( "Invalid chunked data format" );

				// bad request ?
				// delete this
				return ( EXIT_FAILURE );
			}
			else
			{
				this->_request.headers.erase( "transfer-encoding" );
				this->compose_response();
			}
		}
	}
	else if ( this->parse() == EXIT_FAILURE )
	{
		WARN( "HTTP request does not comply with HTTP/1.x specification." );
		this->_buffer_recv.clear();
		if ( this->_status_code == 0 )
			this->setStatusCode( BAD_REQUEST );
		this->compose_response();
		return ( EXIT_FAILURE );
	}

	// Check client_max_body_size limit
	// 0 == unlimited
	if ( this->_server.getClientMaxBodySize( this->_request.target ) != 0
			&& ( this->_request.body.size() > this->_server.getClientMaxBodySize( this->_request.target ) ) )
	{
		this->setStatusCode( CONTENT_TOO_LARGE );
		this->compose_response();
		return ( EXIT_FAILURE );
	}

	if ( this->_request.headers.find( "host" ) != this->_request.headers.end() )
		this->_request.host = this->_request.headers["host"];
	this->_server = this->_router.getServer( this->_request.host,
			this->_connection.getHost(), this->_connection.getPort() );

	// if its an http redirection
	// lcoation block en el compose
	// append location root change if it fits location the target

	// Map request to internal server path.
	this->_request.file = this->_request.target;
	std::string location = this->_server.getRouteString( this->_request.target );
	if ( ! location.empty() && location[0] != '*' )
	{
		std::string root_location = this->_server.getRoot( location );
		root_location.append( "/" );
		this->_request.file.replace( 0 , location.length(), root_location );
	}
	else
	{
		std::string root_location = this->_server.getRoot( location );
		root_location.append( "/" );
		this->_request.file.replace( 0, 1, root_location );
	}
	if ( this->_request.file.back() == '/' )
		this->check_index();
	stat( this->_request.file.c_str(), &this->_request.file_info );

	// Tells HTTP next content coming from socket
	// is the body from this request.
	if ( this->_request.headers.find( "expect" ) != this->_request.headers.end()
			&& this->_request.headers.at( "expect" ).empty() == false )
	{
		this->_expect = true;
		return ( EXIT_SUCCESS );
	}

	return ( this->compute_response() );
}
*/
