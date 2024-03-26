/*                                                                            */
/* Server.hpp                                                                 */
/* mpuig-ma <mpuig-ma@student.42barcelona.com>                                */
/* Tue Feb 13 11:46:18 2024                                                   */

#ifndef _SERVER_HPP_
#define _SERVER_HPP_

#include "IEvent.hpp"
#include "t_address.hpp"
#include "t_server_conf.hpp"

// TODO: make t_server_conf into class ServerConf

class Server: public IEvent
{
	public:

		Server ( void );
		Server& operator= ( t_server_conf & );
		~Server ( void );
		
		bool good ( void ) const;
		int id ( void ) const;
		int start ( void );
		int stop ( void );

		void dispatch ( struct kevent & ev );

		t_server_conf & getConf ( void );

	private:

		bool           _good;
		t_address      _address;
		t_server_conf  _conf;
		
		/*
		void register_read_socket ( void ) const;
		int receive_request ( int64_t );
		std::vector< Client * > _clients;
		*/

};

Server::Server ( void )
{
	DEBUG ( "" );

	return ;
}

Server::~Server ( void )
{
	DEBUG ( "" );
	
	return ;
}

bool
Server::good ( void ) const
{
	return ( this->_good );
}

int
Server::id ( void ) const
{
	return ( this->_address.socket_fd );
}

int
Server::start ( void )
{
	DEBUG ( "" );

	return ( EXIT_SUCCESS );
}

int
Server::stop ( void )
{
	DEBUG ( "" );

	return ( EXIT_SUCCESS );
}

void
Server::dispatch ( struct kevent & ev )
{
	(void) ev;

	return ;
}

t_server_conf &
Server::getConf ( void )
{
	DEBUG ( "" );

	return ( this->_conf );
}

#endif /* !_SERVER_HPP_ */
