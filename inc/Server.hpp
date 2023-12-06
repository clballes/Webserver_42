/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clballes <clballes@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/04 11:56:41 by clballes          #+#    #+#             */
/*   Updated: 2023/12/04 11:56:44 by clballes         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __SERVER_HPP__
#define __SERVER_HPP__

#include <iostream>
#include <map>
#include <sys/socket.h>
#include <netinet/in.h>
#include "ServerOptions.hpp"
#include "Location.hpp"

class Server
{
    public:
        Server();
        Server(const ServerOptions &options);
        Server(Server & src);
        // Server & operator=(Server & src);
        ~Server();
        
        //server functions
        void populateServer();

        //metodes
        // void setLocation();

    private:
        std::map<std::string, std::string> mapConfigServer;
        std::string server_name;
        std::string listen;
        std::string root;
        std::string index;
        std::string allow_methods;
        std::map<Location, std::string> mapLocation;
        struct sockaddr_in server_address;
};

#endif