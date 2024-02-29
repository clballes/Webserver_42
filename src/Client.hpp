/*                                                                            */
/* Client.hpp                                                                 */
/* mpuig-ma <mpuig-ma@student.42barcelona.com>                                */
/* Tue Feb 27 14:32:32 2024                                                   */

#ifndef _CLIENT_HPP_
#define _CLIENT_HPP_

#include "IEvent.hpp"
#include "HTTP.hpp"
#include "webserv.hpp"

class Client: public IEvent
{
	public:

		Client ( const Server & );
		~Client ( void );

		bool good;
		void dispatch ( struct kevent & );

		int register_recv ( void );
		int register_send ( void );

		int request_recv ( int64_t );
		int request_send ( int64_t );

		friend class HTTP;

	private:
	
		int						_socket_fd;
		unsigned				_address_len;
		struct sockaddr_in		_address;

		int64_t					_data_recv;
		int64_t					_data_send;
		char *                  _buffer_recv;
		char *                  _buffer_send;

		HTTP *                  _http_request;
		const Server &          _server;

};

#endif /* !_CLIENT_HPP_ */
