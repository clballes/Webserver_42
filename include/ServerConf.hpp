/*                                                                            */
/* ServerConf.hpp                                                             */
/* mpuig-ma <mpuig-ma@student.42barcelona.com>                                */
/* Wed Apr  3 11:18:43 2024                                                   */

#ifndef _SERVERCONF_HPP_
#define _SERVERCONF_HPP_

#include <string>
#include <sstream>
#include <vector>
#include <map>
#include "debug.hpp"
#include "define.hpp"

class ServerConf
{
	public:

		ServerConf ( void );
		~ServerConf ( void );

		bool getFlag ( int ) const;
		std::size_t getFlags ( void ) const;
		std::size_t getClientMaxBodySize ( void ) const;
		std::string getCGIparam ( void ) const ;
		std::string getCGIpass ( void ) const;
		std::string getRoot ( void ) const;
		std::vector< std::string > getServerNames ( void ) const; //servername_matches fn
		std::vector< std::string > getIndex ( void ) const; // index_matches fn
		std::string getErrorPage ( int );

		int setFlag ( int, bool );
		int setClientMaxBodySize ( std::size_t );
		int setCGIparam ( std::string & );
		int setCGIpass ( std::string & );
		int setRoot ( std::string & );
		int setServerName ( std::string & );
		int setIndex ( std::string & );
		int setErrorPage ( int, std::string & );

		void log_conf ( void );

	private:

		std::size_t									_flags;
		std::size_t									_client_max_body_size;
		std::string									_cgi_param;
		std::string									_cgi_pass;
		std::string									_root;
		std::vector< std::string >					_server_name;
		std::vector< std::string >					_index;
		std::map< int, std::string >				_error_page;
		std::map< std::string,
			std::map< std::string, std::string > >	_locations;

};

#endif /* !_SERVERCONF_HPP_ */
