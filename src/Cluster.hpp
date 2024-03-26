/*                                                                            */
/* Cluster.hpp                                                                */
/* mpuig-ma <mpuig-ma@student.42barcelona.com>                                */
/* Tue Feb 27 14:32:32 2024                                                   */

#ifndef _CLUSTER_HPP_
#define _CLUSTER_HPP_

#include "parse.hpp"
#include "Server.hpp"
#include "ServerHandler.hpp"

class Cluster
{
    public:
        Cluster(void);
        // Cluster(const Cluster & src);
        ~Cluster(void);
        // Cluster & operator=(const Cluster & src);

        int             config(std::string fileconf);   
        int             setup(void);
        int             webserver(void); //stablish the kque
        static  void    graceful_stop ( int n );
        static void     event_loop ( int kq );
        // static void graceful_stop(int signal);
        // void	clean(void);
        // void signal_handler_wrapper(int signal);

    private:
        ServerHandler						_conf;
        static  std::vector<Server>         _server;
        static bool _status;

        // std::map<long, Server>		_servers;
        // std::map<long, Server *>	_sockets;
        // std::vector<int>			_ready;
        // fd_set						_fd_set;
        // unsigned int				_fd_size;
        // long						_max_fd;
};

#endif