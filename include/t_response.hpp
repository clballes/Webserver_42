//                                                                             /
// t_response.hpp                                                              /
// mpuig-ma <mpuig-ma@student.42barcelona.com>                                 /
// Thu May 16 15:47:00 2024                                                    /

#ifndef T_RESPONSE_HPP_
#define T_RESPONSE_HPP_

#include "t_headers.hpp"

typedef struct s_response
{
	std::string		body;
	int				status_code;
	t_headers		headers;

} t_response;

#endif // ! T_RESPONSE_HPP_
