/*                                                                            */
/* webserv.hpp                                                                */
/* mpuig-ma <mpuig-ma@student.42barcelona.com>                                */
/* Mon Jan  8 12:28:55 2024                                                   */

#ifndef _WEBSERV_HPP_
# define _WEBSERV_HPP_

# include "define.hpp"

# include <cstdlib>			/* EXIT_SUCCESS, EXIT_FAILURE */
# include <iostream>		/* cout, cerr, clog */

# include <unistd.h>		/* execve, dup, dup2, pipe, fork,
							   close, read, write, access */
# include <stdio.h>			/* strerror */
# include <sys/errno.h>		/* errno */

# include <netdb.h>			/* common for
							   gai_strerror,
							   getaddrinfo,
							   freeaddrinfo,
							   getprotobynumber */

# include <sys/types.h>		/* common for
							   gai_strerror,
							   kqueue,
							   connect,
							   getaddrinfo,
							   freeaddrinfo,
							   read */

# include <arpa/inet.h>		/* htons, hhtonl, ntohs, ntohl */

# include <sys/select.h>	/* FD_CLR, FD_COPY, FD_ISSET, FD_SET,
							   FD_ZERO, select */

# include <poll.h>			/* poll */
/* include <sys/epoll.h>	epoll... FOT TOUND */

# include <sys/event.h>		/* kqueue, ... */
# include <sys/time.h>		/* kqueue */

# include <netinet/in.h>	/* AF_INET, ... */
# include <sys/socket.h>	/* socket,
							   accept,
							   listen,
							   send,
							   recv,
							   bind,
							   connect,
							   getaddrinfo,
							   freeaddinfo,
							   setsockopt,
							   gettsockname */

# include <fcntl.h>			/* fcntl */
# include <sys/wait.h>		/* waitpid */
# include <signal.h>		/* kill */
# include <sys/stat.h>		/* stat */
# include <dirent.h>		/* opendir, readdir, closedir */

// This needs to be resorted and reordered properly
// ...

# include <sys/cdefs.h>
# include <set>
# include "Server.hpp"

class Server;

__BEGIN_DECLS extern "C"
void webserv ( const std::set< const Server * > & servers );
__END_DECLS

#endif /* webserv.hpp */
