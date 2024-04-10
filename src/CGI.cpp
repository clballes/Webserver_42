/*                                                                            */
/* CGI.cpp                                                                    */
/* mpuig-ma <mpuig-ma@student.42barcelona.com>                                */
/* Tue Apr  9 16:21:17 2024                                                   */

#include "CGI.hpp"

#define READ 0
#define WRITE 1

CGI::CGI ( HTTP & http ): _http( http ), _env( 0x0 )
{
	DEBUG( "" );
	this->_env = new char * [4];
	this->_env[0] = strdup( "REQUEST_METHOD=GET" );
	this->_env[1] = strdup( "PATH_INFO=" );
	this->_env[2] = strdup( "SERVER_PROTOCOL=HTTP/1.1" );
	this->_env[3] = nullptr;
	return ;
}

CGI::~CGI ( void )
{
	DEBUG( "" );
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
	DEBUG( ev.ident );
	if ( ev.flags & NOTE_EXIT )
	{
		char buffer[1024]; // Buffer to read into
		std::string line;
		// Read from the pipe until there's no more data
		ssize_t bytesRead;
		while ( (bytesRead = read( _pipefd[READ], buffer, sizeof( buffer ) ) ) > 0 )
			line.append(buffer, bytesRead);
		LOG( "Contents of line: " << line );
		// this->_http._message_body.append( line.c_str() ); // mha denviar la string
		close( _pipefd[READ] ); 
	}
	return ;
}


int CGI::register_process( pid_t pid)
{
    struct kevent ev;
	
	DEBUG( pid );
    // Set up the event structure for process monitoring
    EV_SET( &ev, pid, EVFILT_PROC,
           EV_ADD | EV_ENABLE | EV_ONESHOT, NOTE_EXIT, 0, (void *) this );
    // Register the event with kqueue
    if ( ::kevent( IEvent::kq, &ev, 1, 0x0, 0, 0 ) == -1 )
	{
		ERROR( PROGRAM_NAME << ": kevent: " << ::strerror( errno ) );
        return ( EXIT_FAILURE );
    }
    return ( EXIT_SUCCESS );
}

int
CGI::execute ( void )
{
	int   fdopen;
    // int   pipefd[2];
	pid_t pid;

	DEBUG( "" );
    fdopen = open( this->_http.getTarget().c_str(), O_RDONLY );
    if ( fdopen == -1 )
	{
        ERROR( this->_http.getTarget() << " " << ::strerror( errno ) );
		return ( EXIT_FAILURE );
    }
    if ( pipe( _pipefd ) == -1 )
	{
        ERROR( "pipe: " << strerror( errno ) );
		return ( EXIT_FAILURE );
    }
    pid = fork();
	if ( pid == -1 )
    {
        ERROR( "fork: " << ::strerror( errno ) );
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
		ERROR( "exec: " << ::strerror( errno ) );
        exit( EXIT_FAILURE );
		// TODO: timeout
    }
    close( _pipefd[WRITE] );
	// register the process in kevent
	this->register_process( pid );
	// close( _pipefd[READ] );
	return ( EXIT_SUCCESS );
}
