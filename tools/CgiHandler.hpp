#ifndef _CGIHANDLER_HPP_
#define _CGIHANDLER_HPP_

#include "webserv.hpp"
#include <map>

class CGI
{
	public:
		CgiHandler ( void );
		~CgiHandler ( void );

		void init_env();

	private:
        std::map<std::string, std::string> _env;
};

#endif
