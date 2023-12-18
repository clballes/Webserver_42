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
#include <list>
#include <sys/socket.h>
#include <netinet/in.h>
#include "ServerOptions.hpp"
#include "Location.hpp"

class Server
{
    public:
        Server(std::list<std::string>list2);
        Server(const ServerOptions &options);
        Server(Server & src);
        // Server & operator=(Server & src);
        ~Server();
        
        //server functions
        void populateServer(std::list<std::string>listServer);
        void populateLocation(std::list<std::string>listServer);

    private:
        std::string server_name;
        std::string listen;
        std::string root;
        std::string index;
        std::string allow_methods;
        // struct sockaddr_in server_address;
};

#endif