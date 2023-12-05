/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clballes <clballes@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/04 11:56:41 by clballes          #+#    #+#             */
/*   Updated: 2023/12/04 11:56:44 by clballes         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __CONFIG_HPP__
#define __CONFIG_HPP__

#include <iostream>
#include <fstream>
#include <sstream>
#include <list>
#include <exception>
#include <string>
#include <map>
#include <vector>

struct Location
{
    std::string path;
    std::string alias;
    std::string root;
    std::string index;
    std::string allow_methods;
    std::string cgi_pass;
    int         client_size;
};

struct Server
{
    std::string server_name;
    std::string listen;
    std::string root;
    std::string index;
    std::string allow_methods;
    std::vector<Location>Locations;
};

class Config
{
    public:
        Config();
        Config(Config & src);
        // Config & operator=(Config & src);
        ~Config();

        //creem el mapa de config pero nomes server
        void ConfigServer(std::string filename);
        void configLocation();  

        //parsing functions
        void parseConfig();
        std::string trim(const std::string& str);
        
        //server functions
        void populateServer();
        const Server& getServer() const;
        
        //exceptions
        class FailOpen : public std::exception
        {
                public:
                        virtual const char * what() const throw();
        };

    private:
        std::string filename;
        Server server;
        std::map<std::string, std::string> mapConfigServer;
        // std::list<std::string>listConfig;
};

#endif