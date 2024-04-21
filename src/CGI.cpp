/*                                                                            */
/* CGI.cpp                                                                    */
/* mpuig-ma <mpuig-ma@student.42barcelona.com>                                */
/* Tue Apr  9 16:21:17 2024                                                   */

#include "CGI.hpp"

#define READ 0
#define WRITE 1

CGI::CGI ( HTTP & http ): _http( http ), _env( 0x0 ), _line("")
{
	DEBUG( "" );
	setmap();
	map_to_arr();
	// PRINTING ENVIRONEMNT CGI
	std::cout << "----------------- Printing char** array:" << std::endl;
    for (int i = 0; this->_env[i] != NULL; ++i) {
        std::cout << this->_env[i] << std::endl;
    }
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

void CGI::setmap()
{
	DEBUG ("Creating the map")
	std::map<std::string, std::string>	headers = _http.getHeaders();
	//no ho tinc gens clar PERQUE NO LA TROBO EN EL HEADER
	// if (headers.find("Auth-Scheme") != headers.end() && headers["Auth-Scheme"] != "")
	// 	this->_envMap["AUTH_TYPE"] = headers["Authorization"];
	// std::cout << "********** Printing Headers:" << std::endl;
	// std::map<std::string, std::string>::iterator iter;
	// for (iter = headers.begin(); iter != headers.end(); ++iter) {
	// 	std::cout << iter->first << ": " << iter->second << std::endl;
	// }
	this->_envMap["REDIRECT_STATUS"] = "200";
	this->_envMap["GATEWAY_INTERFACE"] = "CGI/1.1";
	this->_envMap["SERVER_PROTOCOL"] = "HTTP/1.1";
	this->_envMap["SERVER_SOFTWARE"] = "Webserv/1.0";
	this->_envMap["REQUEST_METHOD"] = _http.getRequest().method->method;
	this->_envMap["QUERY_STRING"] = _http.getRequest().query;

	//just post and put methods
	this->_envMap["CONTENT_TYPE"] = headers["Content-Type"];
	this->_envMap["CONTENT_LENGTH"] = std::to_string(_http.getRequest().body.length());

	// quan faig print dels headers, on estan aquests?
	this->_envMap["REMOTE_IDENT"] = headers["Authorization"];
	this->_envMap["REMOTE_USER"] = headers["Authorization"];

	// this->_envMap["SCRIPT_NAME"] = _http.getRequest().target;
	// this->_envMap["SCRIPT_FILENAME"] = ;

	this->_envMap["PATH_INFO"] = "/Users/clballes/Desktop/web/tests/cgi_tester" ;
	std::cout << " ---------------------- " <<  _http.getCGIpass() << std::endl;
	std::cout << " ---------------------- " << _http.getRequest().target << std::endl;
	
	// this->_envMap["PATH_TRANSLATED"] = ; 
	// this->_envMap["REQUEST_URI"] = ;
	
	// this->_envMap["REMOTEaddr"] = ;


	// 	this->_envMap["SERVER_NAME"] = ;

	// 	this->_env["SERVER_NAME"] = ;
	// this->_env["SERVER_PORT"] = ;
	
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
		ERROR( PROGRAM_NAME << ": kevent: " << std::strerror( errno ) );
        return ( EXIT_FAILURE );
    }
    return ( EXIT_SUCCESS );
}

int
CGI::execute ( void )
{
	int   fdopen;
	pid_t pid;

	DEBUG( "" );
    fdopen = open( this->_http.getRequest().target.c_str(), O_RDONLY );
    if ( fdopen == -1 )
	{
        ERROR( this->_http.getRequest().target << " " << std::strerror( errno ) );
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
        ERROR( "fork: " << std::strerror( errno ) );
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
		ERROR( "exec: " << std::strerror( errno ) );
        exit( EXIT_FAILURE );
		// TODO: timeout
    }
    close( _pipefd[WRITE] );

	// register the process in kevent
	this->register_process( pid );

	return ( EXIT_SUCCESS );
}

void
CGI::dispatch ( struct kevent & ev )
{
	DEBUG( ev.ident );
	if (ev.filter == EVFILT_PROC )
	{
		char buffer[1024]; // Buffer to read into
		std::string line;
		// Read from the pipe until there's no more data
		ssize_t bytesRead;
		while ( (bytesRead = read( _pipefd[READ], buffer, sizeof( buffer ) ) ) > 0 )
		{
			line.append(buffer, bytesRead);
		}
		this->_http.setMessageBody(line);// mha denviar la string
		LOG( "Contents of line: " << line );
		close( _pipefd[READ] );
		this->_http.register_send();
	}
	return ;
}

std::string CGI::getLine()
{
	return this->_line;
}

void CGI::map_to_arr()
{
    this->_env = new char*[this->_envMap.size() * 2 + 1];

    int index = 0;

    for (std::map<std::string, std::string>::const_iterator it = this->_envMap.begin(); it != this->_envMap.end(); ++it) {
        std::string keyValueString = it->first + "=" + it->second;
        this->_env[index] = new char[keyValueString.size() + 1];
        strcpy(this->_env[index], keyValueString.c_str());

        ++index;
    }
    this->_env[index] = NULL;
}
