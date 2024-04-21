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
	setmap();
	map_to_arr();

	// PRINTING ENVIRONEMNT CGI
	std::cout << " ----------------------------------------- " << std::endl;
    for (int i = 0; this->_env[i] != NULL; ++i) {
        std::cout << this->_env[i] << std::endl;
    }
	std::cout << " ----------------------------------------- " << std::endl;
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
	if (headers.find("auth-Scheme") != headers.end() && headers["auth-Scheme"] != "")
		this->_envMap["AUTH_TYPE"] = headers["authorization"];
	this->_envMap["REDIRECT_STATUS"] = "200";
	this->_envMap["GATEWAY_INTERFACE"] = "CGI/1.1";
	this->_envMap["SERVER_PROTOCOL"] = "HTTP/1.1";
	this->_envMap["SERVER_SOFTWARE"] = "Webserv/1.0";
	this->_envMap["REQUEST_METHOD"] = _http.getRequest().method->method;
	this->_envMap["QUERY_STRING"] = _http.getRequest().query;
	this->_envMap["CONTENT_TYPE"] = headers["content-Type"];
	this->_envMap["CONTENT_LENGTH"] = std::to_string(_http.getRequest().body.length());
	this->_envMap["REMOTE_IDENT"] = headers["authorization"];
	this->_envMap["REMOTE_USER"] = headers["authorization"];
	this->_envMap["SCRIPT_NAME"] = _http.getRequest().target;
	this->_envMap["SCRIPT_FILENAME"] =  _http.getRequest().target;
	this->_envMap["PATH_INFO"] =_http.getCGIpass();	
	this->_envMap["PATH_TRANSLATED"] = _http.getCGIpass(); 
	this->_envMap["REQUEST_URI"] = _http.getCGIpass() + _http.getRequest().query;
	this->_envMap["REQUEST_URI"] = _http.getCGIpass();
	if (headers.find("Hostname") != headers.end())
		this->_envMap["SERVER_NAME"] = headers["hostname"];
	else
		this->_envMap["SERVER_NAME"] = this->_envMap["REMOTEaddr"];
	this->_envMap["REMOTEaddr"] = this->_envMap["SERVER_NAME"];

	// fer la funcio ara esta malament aixo
	// this->_envMap["SERVER_NAME"] = std::to_string(_http.getServer()->getHost());
	// this->_envMap["SERVER_PORT"] = _http.getServer()->getPort();	
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

	DEBUG( this->_http.getRequest().target.c_str() << " and "<< _http.getCGIpass().c_str())
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
		// std::cout <<"cgi es:" << _http.getCGIpass().c_str() << std::endl;
        // execve( _http.getCGIpass().c_str(), NULL, this->_env );
		if (strncmp(_http.methods->method, "POST", 5) == 0)
        	execve( "/Users/clballes/Desktop/web/tests/cgi_post.py", NULL, this->_env );
		else if (strncmp(_http.methods->method, "GET", 5) == 0)
        	execve( "/Users/clballes/Desktop/web/cgi-bin/cgi2_script.py", NULL, this->_env );

		ERROR( "exec en el cgi: " << ::strerror( errno ) );
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
		char buffer[1024];
		std::string line;
		ssize_t bytesRead;
		while ( (bytesRead = read( _pipefd[READ], buffer, sizeof( buffer ) ) ) > 0 )
		{
			line.append(buffer, bytesRead);
		}
		parsing_headers( line );
		close( _pipefd[READ] );
		this->_http.register_send();
	}
	return ;
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

void CGI::parsing_headers( std::string line )
{
	std::map <std::string, std::string> mapHeaders;
    size_t pos_final = line.find("\n\n");

    // Separate headers and body
    std::string headers, body;
    if (pos_final != std::string::npos) {
        headers = line.substr(0, pos_final);
        body = line.substr(pos_final + 2);
		this->_http.set_message_body ( body );
    }
	else
		headers = line;

	std::transform(headers.begin(), headers.end(), headers.begin(), ::tolower);
	headers = normalize(headers);
	size_t i = 0;
    while (i < headers.length()) {
        size_t pos = headers.find(":", i);
        if (pos != std::string::npos) {
            std::string str = headers.substr(i, pos - i);
            size_t pos_sp = headers.find(" ", pos + 2);
            std::string str2;
            if (pos_sp != std::string::npos) {
                str2 = headers.substr(pos + 2, pos_sp - pos - 2);
            } else {
                str2 = headers.substr(pos + 2);
            }
            mapHeaders[str] = str2;
            i = pos_sp != std::string::npos ? pos_sp + 1 : pos + 1;
        } else {
            break;
        }
    }

	for (std::map<std::string, std::string>::iterator it = mapHeaders.begin(); it != mapHeaders.end(); ++it)
	{
		if (it->first == "content-type")
			this->_http.set_response_headers ( it->first , it->second);
		if (it->first == "status")
			this->_http.setStatusCode ( std::atoi(it->second.c_str()) );
    }

}
