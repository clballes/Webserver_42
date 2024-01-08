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
        
		void ServerConfList (std::string);
        void parseServerConf (void);

        class FailOpen : public std::exception
        {
                public:
                        virtual const char * what() const throw();
        };

    private:

        std::string					filename;
        std::list<std::string>		listServerConf;

};

# endif /* ServerConf.hpp */
