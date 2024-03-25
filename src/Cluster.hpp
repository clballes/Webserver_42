/*                                                                            */
/* Cluster.hpp                                                                */
/* mpuig-ma <mpuig-ma@student.42barcelona.com>                                */
/* Tue Feb 27 14:32:32 2024                                                   */

#ifndef _CLUSTER_HPP_
#define _CLUSTER_HPP_

#include "Cluster.hpp"
#include "parse.hpp"
#include "ServerHandler.hpp"

class Cluster
{
    public:
        Cluster(void);
        // Cluster(const Cluster & src);
        ~Cluster(void);
        // Cluster & operator=(const Cluster & src);

        int config(std::string fileconf);   
        // int		setup(void);
        // void	run(void);
        // void	clean(void);
    private:
        ServerHandler						_conf;

        // std::map<long, Server>		_servers;
        // std::map<long, Server *>	_sockets;
        // std::vector<int>			_ready;
        // fd_set						_fd_set;
        // unsigned int				_fd_size;
        // long						_max_fd;
};

#endif