/*                                                                            */
/* Server.cpp                                                                 */
/* clballes <clballes@student.42barcelona.com>                                */
/* Mon Jan  8 12:39:29 2024                                                   */

#include "Server.hpp"

const struct addrinfo
Server::_hints =
{
	AI_ADDRCONFIG | AI_PASSIVE, 
	AF_INET, 
	SOCK_STREAM, 
	IPPROTO_TCP, 
	0x0,
	0x0,
	0x0,
	0x0
};

// Initialization functions

Server::Server (void): _status(WEBSERV_OK), _addrinfo_list(0x0)
{
	std::clog << "[" << this << "] ";
    std::clog << "[Server] Constructor called" << std::endl;

	this->_s_address.sin_family = AF_INET;
	(void)_client_max_body_size;
	(void)_hints;

	this->_listen_host.assign("127.0.0.1");

	return ;
}

Server::Server (const Server& instance)
{
	std::clog << "[" << this << "] ";
    std::clog << "[Server] Copy called" << std::endl;
    
	*this = instance;

	return ;
}

Server& 
Server::operator= (const Server& instance)
{
	std::clog << "[" << this << "] ";
	std::clog << "[Server] operator= called" << std::endl;

	this->_status = instance._status;
	this->_s_address = instance._s_address;
	this->_client_max_body_size = instance._client_max_body_size;
	this->_server_name = instance._server_name;
	
	return (*this);
}

Server::~Server (void)
{
	std::clog << "[" << this << "] ";
    std::clog << "[Server] Destructor called" << std::endl;

	if (this->_addrinfo_list != 0x0)
		::freeaddrinfo(this->_addrinfo_list);

	return ;
}

// Member functions

bool
Server::ok (void) const
{
	return (this->_status);
}

void
Server::start (void)
{
	std::clog << "[" << this << "] ";
	std::clog << "::start CALL" << std::endl;

	this->setaddrinfo();
	if (this->_status != 0)
		return ;
	
	this->socket(); 
	if (this->_status != 0)
		return ;
	
	this->bind();
	if (this->_status != 0)
		return ;
	
	this->listen();
	if (this->_status != 0)
		return ;

	return ;
}

void
Server::setaddrinfo (void)
{
	this->_status = ::getaddrinfo(this->_listen_host.c_str(),
			"http",
			&Server::_hints,
			&this->_addrinfo_list);

	if (this->_status != 0)
	{
		std::cerr << "[" << this << "] ";
		std::cerr << ::gai_strerror(this->_status) << std::endl;
		return ;
	}

	std::clog << "[" << this << "] ";
	std::clog << "::setaddrinfo CALL" << std::endl;

	return ;
}

void
Server::socket (void)
{
	int domain = AF_INET, type = SOCK_STREAM, protocol = IPPROTO_TCP;

	if (this->_addrinfo_list == 0x0)
		return ;

	domain = this->_addrinfo_list->ai_family;
	type = this->_addrinfo_list->ai_socktype;
	protocol = this->_addrinfo_list->ai_protocol;

	std::clog << "[" << this << "] ";
	std::clog << "::socket CALL" << std::endl;
	
	std::clog << "[" << this << "] ";
	std::clog << "::socket "
		<< "domain: " << domain << std::endl;

	std::clog << "[" << this << "] ";
	std::clog << "::socket "
		<< "type: " << type << std::endl;
	
	std::clog << "[" << this << "] ";
	std::clog << "::socket "
		<< "protocol: " << protocol << std::endl;

	this->_socket = ::socket(domain, type, protocol);

	std::clog << "[" << this << "] ";
	std::clog << "::socket "
		<< "assigned: " << std::dec << this->_socket << std::endl;

	if (this->_socket == -1)
	{
		std::cerr << "[" << this << "] ";
		std::cerr << ::strerror(errno) << std::endl;
		this->_status = errno;
		return ;
	} // will need to close socket, later

	return ;
}

void
Server::bind (void)
{
	std::clog << "[" << this << "] ";
	std::clog << "::bind CALL" << std::endl;

	if (::bind(this->_socket,
			this->_addrinfo_list->ai_addr,
			this->_addrinfo_list->ai_addrlen) == -1)
	{
		std::cerr << "[" << this << "] ";
		std::cerr << ::strerror(errno) << std::endl;
		this->_status = errno;
		return ;
	}

	std::clog << "[" << this << "] ";
	std::clog << "::bind OK " << std::endl;

	std::clog << "[" << this << "] ";
	std::clog << "::bind addr: ";
	if (this->_addrinfo_list->ai_addr->sa_family == AF_INET)
		std::clog << "IPv4";
	else
		std::clog << "IPv6";
	std::clog << std::endl;

	return ;
}

void
Server::listen (void)
{
	std::clog << "[" << this << "] ";
	std::clog << "::listen" << std::endl;
	
	// will need to
	// set default value for backlog. 0 for now
	if (::listen(this->_socket, 0) == -1)
	{
		std::cerr << ::strerror(errno) << std::endl;
		return ;
	}


	return ;
}

void
Server::accept (void)
{
	return ;
}

const struct in_addr&
Server::getAddr (void) const
{
	return (this->_s_address.sin_addr);
}

void
Server::setAddr (uint32_t ip)
{
	std::clog << "[" << this << "] ";
	std::clog << "SET address to 0x" << std::hex << ip << std::endl;
	
	this->_s_address.sin_addr.s_addr = ip;
	(void) this->_s_address;

	return ;
}

int
Server::getPort (void) const
{
	return (ntohs(this->_s_address.sin_port));
}

void
Server::setPort (int port)
{
	std::clog << "[" << this << "] ";
	std::clog << "SET port to " << port << std::endl;

	this->_s_address.sin_port = htons(port);
	
	return ;
}

const std::string&
Server::getName (void) const
{
	return (this->_server_name);
}

void
Server::setName (const std::string& name)
{
	std::clog << "[" << this << "] ";
	std::clog << "SET server_name to " << name << std::endl;
	
	this->_server_name.assign(name);

	return ;
}

#include <arpa/inet.h>

std::ostream&
operator<< (std::ostream& ofs, const Server& instance)
{
	char	str[INET_ADDRSTRLEN];

	ofs << "port: " << instance.getPort() << std::endl;
	ofs << "address: " << inet_ntop(AF_INET, &(instance.getAddr()),
			str, INET_ADDRSTRLEN);
	
	return (ofs);
}
