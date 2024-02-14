/*                                                                            */
/* init.c                                                                     */
/* mpuig-ma <mpuig-ma@student.42barcelona.com>                                */
/* Tue Feb 13 19:55:11 2024                                                   */

#include <getopt.h>
#include "getoptions.h"
#include "webserv.hpp"

int
decode_arguments ( int argc, char * const * argv )
{
	/* all this goes into init_argv () */
	int						c = 0, cc = 0;
	int 					option_index = 0;
	const char *			shortopts = "vm:l:";
	
	const struct option		longopts[] =
	{
		{ "version", no_argument, 0x0, 'v'}, //{ "verbose", no_argument, &f_verbose, 'V'},
		{ "log", required_argument, 0x0, 'l'},
		{ "mode", required_argument, 0x0, 'm'},
		{ 0, 0, 0, 0 }
	};
	
	while ( c != -1 )
	{
		c = getopt_long ( argc, argv, shortopts, longopts, &option_index );
		(void) cc;
		//cc = getoptions_long ( argc, argv, shortopts, longopts, &option_index );

		switch ( c )
		{
			case 'v':
				std::cout << WEBSERV_VER << std::endl;
				return (EXIT_SUCCESS);
			case 'l':
				//std::ofstream log_file;
				//log_file.open("logs.txt", std::ostream::app);
				//std::clog.rdbuf(log_file.rdbuf());
				std::cout << "logging into " << optarg << std::endl;
				break ;
			case 'm':
				std::cout << "mode: " << optarg << std::endl;
				break ;
			case '?':
				return (EXIT_FAILURE);
			default:
				break ;
		}
	}
	/* _END OF all this */
	return (0);
}
