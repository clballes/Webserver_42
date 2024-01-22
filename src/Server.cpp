/*                                                                            */
/* Server.cpp                                                                 */
/* clballes <clballes@student.42barcelona.com>                                */
/* Mon Jan  8 12:39:29 2024                                                   */

#include "Server.hpp"

// ---------------- INIZIALIZATION FUNCTIONS -----------------------

Server::Server (void): _status(WEBSERV_OK)
{
	std::clog << "[" << this << "] ";
    std::clog << "[Server] Constructor called" << std::endl;

	this->_s_address.sin_family = AF_INET;
	(void) _client_max_body_size;

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

	return ;
}

// ----------------------- MEMBER FUNCTIONS ----------------------

bool
Server::ok (void) const
{
	return (this->_status);
}

void
Server::listen (void) const
{
	int				kq;
	int				new_events;
	struct kevent*	event_list = 0x0;

	std::clog << "[" << this << "] ";
	std::clog << "started listenning()" << std::endl;


	// Prepare the kqueue
	kq = kqueue();

	// Set event(s) to opened sockets
	EV_SET( , socketfd_2_listen, EVILT_READ, EV_ADD | EV_ENABLE, 0, 0, 0);

	// Event loop
	while (this->_status == WEBSERV_OK)
	{
		new_events = kevent(kq, 
	}

	return ;
}

void
Server::populateServer (std::list<std::string>listServer)
{
    std::list<std::string>::iterator itB = listServer.begin();
    std::list<std::string>::iterator itLoc = listServer.end();
    for (; itB != listServer.end(); ++itB) {
        if (itB->find("location") != std::string::npos) {
            itLoc = itB;
            break;
        }
    }
    std::list<std::string>::iterator it = listServer.begin();
    for(; it != itLoc; ++it){
        if(it->find("server_name") != std::string::npos){
            this->_server_name = it->substr(12, it->length());
        }
        else if(it->find("listen") != std::string::npos){
            this->_listen = it->substr(7, it->length());
        }
        else if(it->find("root") != std::string::npos){
            this->_root = it->substr(5, it->length());
        }
        else if(it->find("index") != std::string::npos){
            this->_index = it->substr(6, it->length());
        }
        else if(it->find("allow_methods") != std::string::npos)
        {
            this->_allow_methods = it->substr(14, it->length());
        }
    }
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
