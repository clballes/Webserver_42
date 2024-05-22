//                                                                             /
// HTTP_response.cpp                                                           /
// mpuig-ma <mpuig-ma@student.42barcelona.com>                                 /
// Thu May 16 12:31:40 2024                                                    /

#include "HTTP.hpp"

// There is no need to check if this->_request.method is NULL
// as this check is done in the parse() call;

void clear_request ( t_request & );
void clear_response ( t_response & );

int
HTTP::compute_response ( void )
{
	if ( this->_server.getFlag( this->_request.method->code,
				this->_request.target ) == false )
		this->_response.status_code = METHOD_NOT_ALLOWED;
	else if ( ! this->_server.getCGIpass( this->_request.target ).empty() )
	{
		this->_cgi_ptr = new CGI( *this, this->_server );
		if ( this->_cgi_ptr->execute() == EXIT_SUCCESS )
			return ( EXIT_SUCCESS );
	}
	else if ( !this->_server.getRedirection( this->_request.target ).second.empty() )
	{
		this->_response.status_code = this->_server.getRedirection( this->_request.target ).first;
		this->_response.headers["location"] = this->_server.getRedirection( this->_request.target ).second;
	}
	else
		(void) this->_request.method->method_func( this );
	(void) this->compose_response();
	return ( EXIT_SUCCESS );
}

int
HTTP::compose_response ( void )
{
	if ( this->_response.status_code >= 300 && this->_response.status_code <= 511
			&& this->_request.method != NULL && this->_request.method->code != HTTP_HEAD )
		load_file( this->_server.getErrorPage( this->_response.status_code ),
			   this->_response.body );
	this->_buffer_send.append( "HTTP/1.1 " );
	this->_buffer_send.append( my_to_string( this->_response.status_code ) );
	this->_buffer_send.append( " \r\n" );
	this->_response.headers["content-length"] = my_to_string( this->_response.body.size() );
	if ( this->_request.method != NULL && this->_request.method->code == HTTP_HEAD )
		this->_response.headers.erase( "content-length" );
	// Add response headers if any + ending CRLF.
	for ( t_headers::iterator it = this->_response.headers.begin();
			it != this->_response.headers.end(); ++it )
	{
		this->_buffer_send.append( it->first );
		this->_buffer_send.append( ": " );
		this->_buffer_send.append( it->second );
		this->_buffer_send.append( "\r\n" );
	}
	this->_buffer_send.append( "\r\n" );
	// Add [message body] if any.
	this->_buffer_send.append( this->_response.body );
	this->_buffer_recv.clear();
	this->send_response();
	return ( EXIT_SUCCESS );
}

int
HTTP::send_response ( void )
{
	::send( this->_socket_fd, this->_buffer_send.c_str(),
			this->_buffer_send.length(), 0x0 );
	if ( this->_request.headers.find( "connection" ) != this->_request.headers.end()
			&& this->_request.headers.at( "connection" ) != "keep-alive" )
	{
		delete this;
		return ( EXIT_SUCCESS );
	}
	if ( this->_response.headers.find( "connection" ) != this->_response.headers.end()
			&& this->_response.headers.at( "connection" ) == "close" )
	{
		delete this;
		return ( EXIT_SUCCESS );
	}
	clear_request( this->_request );
	clear_response( this->_response );
	this->_state = PENDING_START_LINE;
	this->_buffer_send.clear();
	return ( EXIT_SUCCESS );
}
