/*                                                                            */
/* ServerConf.hpp                                                             */
/* clballes <clballes@student.42barcelona.com>                                */
/* Mon Jan  8 12:56:22 2024                                                   */

#ifndef _SERVER_CONF_HPP_
# define _SERVER_CONF_HPP_

# include <iostream>
# include <fstream>
# include <list>
# include <exception>
# include <string>

class ServerConf
{
    public:

        ServerConf (void);
        ServerConf (ServerConf&);
        ServerConf& operator= (ServerConf&);
        ~ServerConf (void);

		// la llista fora, al main, no?
		void ServerConfList (std::string);
        void parseServerConf (void);

        class FailOpen : public std::exception
        {
                public:
                        virtual const char * what() const throw();
        };

		void	setPort (int); // must use htons()
		void	setAddr (int); // rep INADDR_ANY (macro)
		int		getPort (void) const;
		int		getAddr (void) const;

    private:

		// struct sockaddr_in			s_address;
		// s_address.sin_family rep AF_INET (macro)
		// s_address.sin_port rep htons(int)
		// s_address.sin_addr.s_addr rep INADDR_ANY (macro), de moment
		std::size_t					client_max_body_size; // 0 == infinit
		std::string					server_name;
        std::string					filename;
        std::list<std::string>		listServerConf;

};

# endif /* ServerConf.hpp */
