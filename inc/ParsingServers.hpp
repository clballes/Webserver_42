/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*  ParsingServers.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clballes <clballes@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/04 11:56:41 by clballes          #+#    #+#             */
/*   Updated: 2023/12/04 11:56:44 by clballes         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __PARSINGSERVERS_HPP__
#define __PARSINGSERVERS_HPP__

#include <iostream>
#include <list>
#include <vector>
#include <fstream>

class ParsingServers
{
    public:
        //canonical form 
        ParsingServers(std::string filename);
        ParsingServers(ParsingServers & src);
        // ParsingServers & operator=(ParsingServers & src);
        ~ParsingServers();

        //metodes
        std::string trim3(const std::string& input);
        std::string trim2(const std::string& input);
        void        printArray();
        void        parsingBrackets(std::string &line);
        void        numServers();
        void        splitServer();

        //get methods
        std::vector<std::list<std::string>*> getArrList();
        unsigned long int getNumServer();

    private:
        std::list<std::string> listConfig;
        std::vector<std::list<std::string>*> arrayOfLists;
        unsigned long int servers;
};

#endif