/*                                                                            */
/* handler.cpp                                                                */
/* clballes <clballes@student.42barcelona.com>                                */
/* Mon Mar  4 15:43:13 2024                                                   */

#include "CGI.hpp"
#include "ft_stdlib.h"

#define READ 0
#define WRITE 1

void
CGI::dispatch ( void )
{
	LOG( "call CGI::dispatch()" );

	return ;
}

//int
//CGI::execute ( const std::string& scriptPath, char **env )

int
CGI::execute ( void )
{
	int   fdopen;
    int   pipefd[2];
	pid_t pid;

	LOG( "call CGI::execute()" );

	// TODO: should come from http.target
	std::string target;
	
	target.append( this->_http._server._root );
	target.append( this->_http._request.target );

    fdopen = open( target.c_str(), O_RDONLY );
    if ( fdopen == -1 )
	{
        std::cerr << target << " ";
		std::cerr << ::strerror( errno );
		std::cerr << std::endl;
		return ( EXIT_FAILURE );
    }

    if ( pipe( pipefd ) == -1 )
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
    
	std::clog << "waitpid: " << waitpid( pid, NULL, 0 );
	std::clog << std::endl;
   
	std::string line;
	
	while ( read( pipefd[READ], (void*)line.data(), line.capacity() ) > 0 )
	{
		//std::clog << "buff ";
		//LOG_BUFFER( line.c_str() );
		this->_http._message_body.append( line.c_str() );
	}
	
	close( pipefd[READ] );

	return ( EXIT_SUCCESS );
}
