/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerOptions.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clballes <clballes@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/04 11:56:41 by clballes          #+#    #+#             */
/*   Updated: 2023/12/04 11:56:44 by clballes         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __SERVEROPTIONS_HPP__
#define __SERVEROPTIONS_HPP__

#include <iostream>
#include <fstream>
#include <sstream>
#include <exception>
#include <string>
#include <map>
#include <list>
#include "Location.hpp"

class ServerOptions
{
    public:
        ServerOptions();
        ServerOptions(std::string filename); //fem open i posem tot en una llista
        ServerOptions(ServerOptions & src);
        // ServerOptions & operator=(ServerOptions & src);
        ~ServerOptions();

        //creem el mapa de Server pero nomes server
        void configServer();
        void configLocation();  

        //parsing functions
        void parseConfigFile();
        std::string trim(const std::string& str);

        //exceptions
        class FailOpen : public std::exception
        {
            public:
                    virtual const char * what() const throw();
        };

    private:
        // std::map<std::string, std::string> mapOptionsServer;
        std::list<std::string> listConfig;
        // int servers; to knnow how many objects we have to create with the info concrete
};

#endif