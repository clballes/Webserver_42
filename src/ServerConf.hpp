/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clballes <clballes@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/04 11:56:41 by clballes          #+#    #+#             */
/*   Updated: 2024/01/08 12:39:12 by mpuig-ma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
        ServerConf();
        ServerConf(ServerConf & src);
        // ServerConf & operator=(ServerConf & src);
        ~ServerConf();
        void ServerConfList(std::string filename);
        void parseServerConf();

        class FailOpen : public std::exception
        {
                public:
                        virtual const char * what() const throw();
        };

    private:
        std::string filename;
        std::list<std::string>listServerConf;

};

# endif /* ServerConf.hpp */
