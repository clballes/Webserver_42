/*                                                                            */
/* CGI.hpp                                                                    */
/* mpuig-ma <mpuig-ma@student.42barcelona.com>                                */
/* Wed Mar 20 12:16:57 2024                                                   */

#ifndef _WEBSERV_CGI_HPP_
#define _WEBSERV_CGI_HPP_

#include "IEvent.hpp"
#include "Log.hpp"

//NO TENIM FETA LA FUNCIO DE REALLOC, no va aplicarho a la libft
//ft_setenv("REQUEST_METHOD", method.c_str(), 1, &envp);
//ft_setenv("PATH_INFO", target.c_str(), 1, &envp);
//ft_setenv("SERVER_PROTOCOL", "HTTP/1.1", 1, &envp);

class HTTP;

class CGI
{
	public:

		CGI ( HTTP & );
		~CGI ( void );

		void dispatch ( void );
		int execute ( void );

		friend class HTTP;

	protected:

		HTTP &  _http;
		char ** _env;

};

#endif /* CGI.hpp */
