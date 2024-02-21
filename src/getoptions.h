/*                                                                            */
/* getoptions.h                                                               */
/* mpuig-ma <mpuig-ma@student.42barcelona.com>                                */
/* Tue Feb 13 20:08:46 2024                                                   */

#ifndef _GETOPTIONS_H_
#define _GETOPTIONS_H_

#include <sys/cdefs.h>
#include <iostream>

__BEGIN_DECLS extern "C"

# ifndef _GETOPT_H_
# define _GETOPT_H_

# define no_argument		0
# define required_argument	1
# define optional_argument	2

struct option {
	const char * name;
	int	has_arg;
	int * flag;
	int val;
};

#  ifndef _GETOPT
#  define _GETOPT

extern int optind;
extern int opterr;
extern int optopt;
extern char * optarg;

#  endif /* !_GETOPT */
# endif /* !_GETOPT_H_ */

int	getoptions_long ( int argc,
		char * const * argv,
		const char * shortopts,
		const struct option * longopts,
		int * option_index );

__END_DECLS

#endif /* !_GETOPTIONS_H_ */
