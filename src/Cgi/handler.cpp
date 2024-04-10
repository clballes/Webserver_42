/*                                                                            */
/* handler.cpp                                                                */
/* clballes <clballes@student.42barcelona.com>                                */
/* Mon Mar  4 15:43:13 2024                                                   */

#include "CGI.hpp"
#include "ft_stdlib.h"

#define READ 0
#define WRITE 1

void
CGI::dispatch ( struct kevent & ev )
{
	LOG( "call CGI::dispatch() (process=" << ev.ident << ")" );

	if ( ev.flags & EVFILT_READ ) //process stop
	{
		std::cout << " ---------------------------------------------------------- - -  - -- - - - -- -      --   - - --entruuuuuuuuuuu "<< std::endl;
		// this->request_send( ev.data );
	}
}


int CGI::register_process()
{
	LOG( "call CGI::register_recv() (process=" << this->_cgi_pid << ")" );

    struct kevent ev;

    // Set up the event structure for process monitoring
    EV_SET(&ev, this->_cgi_pid, EVFILT_PROC,
           EV_ADD | EV_ENABLE | EV_ONESHOT, NOTE_EXIT, 0, (void *)this);

    // Register the event with kqueue
    if (::kevent(IEvent::kq, &ev, 1, 0x0, 0, 0) == -1) {
        std::cerr << PROGRAM_NAME;
		std::cerr << ": kevent: " << ::strerror(errno);
		std::cerr << std::endl;
        return (EXIT_FAILURE);
    }
    return (EXIT_SUCCESS);
}

int
CGI::execute ( void )
{
	int   fdopen;
    int   pipefd[2];

	LOG( "call CGI::execute()" );

    fdopen = open( this->_http._request.target.c_str(), O_RDONLY );
	if ( fdopen == -1 )
	{
        std::cerr << this->_http._request.target << " ";
		std::cerr << ::strerror( errno );
		std::cerr << std::endl;
		return ( EXIT_FAILURE );
    }

    if ( pipe( pipefd ) == -1 )
	{
        std::cerr << "pipe: " << strerror( errno ) << std::endl;
		return ( EXIT_FAILURE );
    }

    this->_cgi_pid = fork();
	if ( this->_cgi_pid == -1 )
    {
        std::cerr << "fork: ";
		std::cerr << ::strerror( errno );
		std::cerr << std::endl;
		// TODO: should not exit
		return ( EXIT_FAILURE );
    }

    if ( this->_cgi_pid == 0 )
    {
		
        close( pipefd[READ] );
		dup2( pipefd[WRITE], STDOUT_FILENO );
        dup2( fdopen, STDIN_FILENO );

		close( pipefd[WRITE] );

        execve( this->_http._server._cgi_pass.c_str(), NULL, this->_env );

		std::cerr << "exec: ";
		std::cerr << ::strerror( errno );
		std::cerr << std::endl;
        exit( EXIT_FAILURE );
		
		// TODO: timeout
    }

    close( pipefd[WRITE] );
    
	// register the process in kevent
	this->register_process();
	// std::clog << "waitpid: " << waitpid( this->_cgi_pid, NULL, 0 ) << "\n";
   
	std::string line;
	char buffer[1024]; // Buffer to read into

	// Read from the pipe until there's no more data
	ssize_t bytesRead;
	while ((bytesRead = read(pipefd[READ], buffer, sizeof(buffer))) > 0) {
		this->_http._message_body.append( line.c_str() );
		line.append(buffer, bytesRead); // Append the read data to the string
	}
	std::clog << "Contents of line: " << line << std::endl;

	close( pipefd[READ] );

	return ( EXIT_SUCCESS );
}
