/*                                                                            */
/* getoptions.cpp                                                             */
/* mpuig-ma <mpuig-ma@student.42barcelona.com>                                */
/* Wed Feb 14 09:47:52 2024                                                   */

#include "getoptions.h"

int
getoptions_long ( int argc,
		char * const * argv,
		const char * shortopts,
		const struct option * longopts,
		int * option_index )
{
	(void) argc;
	(void) argv;
	(void) shortopts;
	(void) longopts;
	(void) option_index;
	// optind
	// opterr
	// optopt
	// optarg
	
	int i;

	i = optind;
	while ( i < argc )
	{
		std::cout << argv[i] << std::endl;
		++i;
	}

	return (0x0);
}
