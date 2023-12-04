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
#include <list>
#include <exception>
#include <string>

class Config
{
    public:
        Config();
        Config(Config & src);
        // Config & operator=(Config & src);
        ~Config();
        void ConfigList(std::string filename);
        void parseConfig();

        class FailOpen : public std::exception
        {
                public:
                        virtual const char * what() const throw();
        };

    private:
        std::string filename;
        std::list<std::string>listConfig;

};

#endif