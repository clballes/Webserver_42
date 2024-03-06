/*                                                                            */
/* CgiHandler.hpp                                                             */
/* mpuig-ma <mpuig-ma@student.42barcelona.com>                                */
/* Tue Feb 27 14:32:20 2024                                                   */

#ifndef _CGIHANDLER_HPP_
#define _CGIHANDLER_HPP_

#include "../webserv.hpp"
#include "../HTTP.hpp"
#include <map>

class CgiHandler
{
	public:
		CgiHandler ( void );
		~CgiHandler ( void );

		char ** init_env(const std::string& scriptPath, const std::string& fullPath, const std::string& query);
		void executeCGI(const std::string& scriptPath, char **env);

	private:
        std::map<std::string, std::string> _env;
};

#endif