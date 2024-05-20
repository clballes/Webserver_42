//                                                                             /
// HTTP_request.cpp                                                            /
// mpuig-ma <mpuig-ma@student.42barcelona.com>                                 /
// Thu May 16 12:31:49 2024                                                    /

#include "HTTP.hpp"

static bool isComplete ( const std::string &, const t_request & );
static void translate_target ( HTTP & );
static void check_index ( HTTP & );

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
	// TODO: expect header
	if ( isComplete( this->_buffer_recv, this->_request ) == true )
	{
		/*
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
		*/
		if ( this->parse() == EXIT_FAILURE )
		{
			WARN( "HTTP request does not comply with HTTP/1.x specification." );
			this->_buffer_recv.clear();
			if ( this->_response.status_code == 0 )
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
		{
			this->_request.host = this->_request.headers.at( "host" );
		}
		this->_server = this->_router.getServer( this->_request.host,
				this->_connection.getHost(), this->_connection.getPort() );
		// if its an http redirection
		// lcoation block en el compose
		// append location root change if it fits location the target
		translate_target( *this );
		check_index ( *this );
		/*
		// Tells HTTP next content coming from socket
		// is the body from this request.
		if ( this->_request.headers.find( "expect" ) != this->_request.headers.end()
				&& this->_request.headers.at( "expect" ).empty() == false )
		{
			this->_expect = true;
			return ( EXIT_SUCCESS );
		}
		*/
		return ( this->compute_response() );
	}
	return ( EXIT_SUCCESS);
}

static bool
isComplete ( const std::string & buffer, const t_request & request )
{
	if ( buffer.find( "\r\n\r\n" ) != std::string::npos )
		return ( true );
	(void) request;
	return ( false );
}

static void
translate_target ( HTTP & http )
{
	const Server &	server = http.getServer();
	t_request &		request = http.getRequest();
	std::string		location, root_location;

	// Map request to internal server path.
	request.file = request.target;
	location = server.getRouteString( request.target );
	if ( ! location.empty() && location[0] != '*' )
	{
		root_location = server.getRoot( location );
		root_location.append( "/" );
		request.file.replace( 0 , location.length(), root_location );
	}
	else
	{
		root_location = server.getRoot( location );
		root_location.append( "/" );
		request.file.replace( 0, 1, root_location );
	}
	stat( request.file.c_str(), &request.file_info );
	if ( S_ISDIR( request.file_info.st_mode ) )
		check_index( http );

	return ;
}

static void
check_index ( HTTP & http )
{
	const Server &	server = http.getServer();
	t_request &		request = http.getRequest();
	std::string		temp;
	const std::vector< std::string > & vec = server.getIndex( request.target );

	for ( std::vector< std::string >::const_iterator it = vec.begin();
			it != vec.end(); ++it )
	{
		temp = request.file;
		if ( temp.back() != '/' )
			temp.append( "/" );
		temp.append( *it );
		if ( routeExists( temp ) )
		{
			request.target.append( "/" );
			request.file.append( *it );
			return ;
		}
	}
	return ;
}
