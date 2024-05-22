//                                                                             /
// t_http_method.hpp                                                           /
// mpuig-ma <mpuig-ma@student.42barcelona.com>                                 /
// Thu May 16 15:45:11 2024                                                    /

#ifndef T_HTTP_METHOD_HPP_
#define T_HTTP_METHOD_HPP_

typedef struct s_http_method
{
	const char *	method;
	int				( *method_func )( void * HTTP );
	int				code;

} t_http_method;

#endif // ! T_HTTP_METHOD_HPP_
