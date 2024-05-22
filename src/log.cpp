//                                                                             /
// log.cpp                                                                     /
// mpuig-ma <mpuig-ma@student.42barcelona.com>                                 /
// Thu May  9 13:06:29 2024                                                    /

#include "log.hpp"

int *
__w_verbose_level ( void )
{
	static int verbose_level = 0;
	return ( &verbose_level );
}
