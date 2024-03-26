/*                                                                            */
/* t_address.hpp                                                              */
/* mpuig-ma <mpuig-ma@student.42barcelona.com>                                */
/* Tue Mar 26 10:51:25 2024                                                   */

#ifndef _T_ADDR_HPP_
#define _T_ADDR_HPP_

#include <netinet/in.h>

// TODO: rename address so its not repeated

struct s_address
{
	int socket_fd;
	unsigned len;
	struct sockaddr_in address;
};

typedef struct s_address t_address;

#endif /* !_T_ADDR_HPP_ */
