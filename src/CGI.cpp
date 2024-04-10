/*                                                                            */
/* CGI.cpp                                                                    */
/* mpuig-ma <mpuig-ma@student.42barcelona.com>                                */
/* Tue Apr  9 16:21:17 2024                                                   */

#include "CGI.hpp"
#define READ 0
#define WRITE 1

CGI::CGI ( HTTP & http ): _http( http ), _env( 0x0 )
{
	LOG( "call CGI::CGI( HTTP & )" );

	this->_env = new char * [4];
	this->_env[0] = strdup( "REQUEST_METHOD=GET" );
	this->_env[1] = strdup( "PATH_INFO=" );
	this->_env[2] = strdup( "SERVER_PROTOCOL=HTTP/1.1" );
	this->_env[3] = nullptr;

	return ;
}

CGI::~CGI ( void )
{
	LOG( "call CGI::~CGI()" );

	if ( this->_env == 0x0 )
		return ;

	for ( std::size_t i = 0; this->_env[i] != 0x0; ++i )
		delete ( this->_env[i] );

	delete ( this->_env );

	return ;
}

void
CGI::dispatch ( struct kevent & ev )
{
	LOG( "call CGI::dispatch() (process=" << ev.ident << ")" );

	if ( ev.flags & NOTE_EXIT ) //process stop
	{
		std::cout << " ---- entruuuuuuuuuuu "<< std::endl;
		char buffer[1024]; // Buffer to read into
		std::string line;
		// Read from the pipe until there's no more data
		ssize_t bytesRead;
		while ((bytesRead = read(_pipefd[READ], buffer, sizeof(buffer))) > 0) {
			line.append(buffer, bytesRead);
		}
		std::clog << "Contents of line: " << line << std::endl;
		// this->_http._message_body.append( line.c_str() ); // mha denviar la string
		close( _pipefd[READ] ); 
	}
}


int CGI::register_process( pid_t pid)
{
	LOG( "call CGI::register_recv() (process=" << pid << ")" );

    struct kevent ev;

    // Set up the event structure for process monitoring
    EV_SET(&ev, pid, EVFILT_PROC,
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
    // int   pipefd[2];
	pid_t pid;

	LOG( "call CGI::execute()" );

    fdopen = open( this->_http.getTarget().c_str(), O_RDONLY );
    if ( fdopen == -1 )
	{
        std::cerr << this->_http.getTarget() << " ";
		std::cerr << ::strerror( errno );
		std::cerr << std::endl;
		return ( EXIT_FAILURE );
    }

    if ( pipe( _pipefd ) == -1 )
	{
        std::cerr << "pipe: " << strerror( errno ) << std::endl;
		return ( EXIT_FAILURE );
    }

    pid = fork();

	if ( pid == -1 )
    {
        std::cerr << "fork: ";
		std::cerr << ::strerror( errno );
		std::cerr << std::endl;
		
		// TODO: should not exit
        
		return ( EXIT_FAILURE );
    }

    if ( pid == 0 )
    {
		
        close( _pipefd[READ] );
        
		dup2( _pipefd[WRITE], STDOUT_FILENO );
        dup2( fdopen, STDIN_FILENO );

		close( _pipefd[WRITE] );

        execve( _http.getCGIpass().c_str(), NULL, this->_env );

		std::cerr << "exec: ";
		std::cerr << ::strerror( errno );
		std::cerr << std::endl;
        exit( EXIT_FAILURE );
		
		// TODO: timeout
    }

    close( _pipefd[WRITE] );
    
	// register the process in kevent
	this->register_process( pid );
	// close( _pipefd[READ] );

	return ( EXIT_SUCCESS );
}
