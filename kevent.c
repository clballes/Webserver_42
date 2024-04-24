#include <sys/types.h>
#include <sys/event.h>
#include <sys/time.h>
#include <printf.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

int
main ( void )
{
	pid_t			pid;
	int 			kq = 0;
	int				n_events = 0;
	struct kevent	ev;

	kq = kqueue();
	if ( kq == -1 )
	{
		printf( "kqueue: %s\n", strerror( errno ) );
		return ( 1 );
	}
	pid = fork();
	if ( pid == -1 )
	{
		printf( "fork: %s\n", strerror( errno ) );
		return ( 1 );
	}
	else if ( pid == 0 )
	{
		execve( "/Users/mpuig-ma/Documents/webserv/tests/while", NULL, NULL );
		printf( "while: error \n" );
		exit ( 1 );
	}
	EV_SET( &ev, pid, EVFILT_PROC, EV_ADD | EV_ENABLE | EV_ONESHOT,
			NOTE_EXIT | NOTE_EXITSTATUS, 0, 0x0 );
    if ( kevent( kq, &ev, 1, 0x0, 0, 0x0 ) == -1 )
	{
		printf( "kevent: %s\n", strerror( errno ) );
		return ( 1 );
	}
	else
	{
		printf( "%d registered\n", pid );
		struct kevent event;
		while ( n_events >= 0 )
		{
			n_events = kevent( kq, 0x0, 0, &event, 1, 0 );
			if ( n_events == 1 )
			{
				printf( "event\n" );
			}
			else if ( n_events == 0 )
				continue ;
		}
	}
	return ( 0 );
}
