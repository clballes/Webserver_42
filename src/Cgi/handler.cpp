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

	std::string scriptPath( "/Users/mpuig-ma/Documents/webserv/tests/cgi_script.py" );
    
	LOG( "Execute CGI" );
    
    fdopen = open( "/Users/clballes/Desktop/web42/www/100.html", O_RDONLY );
    if ( fdopen == -1 )
	{
        std::cerr << "cgi: ";
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

    if ( pid == 0 )
    {
        dup2( fdopen, STDIN_FILENO );
        //dup2( pipefd[WRITE], STDOUT_FILENO );
        close( pipefd[READ] );

        execve( scriptPath.c_str(), NULL, this->_env );
		//el scriptpath estabe aqui es el cgi pass
        
		std::cerr << "exec: ";
		std::cerr << ::strerror( errno );
		std::cerr << std::endl;
        exit( EXIT_FAILURE );
		
		// TODO: timeout
    }
    else if ( pid == -1 )
    {
        std::cerr << "fork: ";
		std::cerr << ::strerror( errno );
		std::cerr << std::endl;
		
		// TODO: should not exit
        
		return ( EXIT_FAILURE );
    }

    write( pipefd[WRITE], "<html><title>CGI Script</title></html>", 39 );
    close( pipefd[WRITE] );
    
	char buff[1024];
	// read(pipefd[READ], &buff, 1024);
    memset( &buff, 0, 1024 );

    std::cout << "el read es: " << read( pipefd[READ], &buff, 1024 ) << std::endl;
    std::cout << "buff es: "<< buff << std::endl;
    // close( pipefd[READ] );
    waitpid( pid, NULL, 0 );

	return ( EXIT_SUCCESS );
}
