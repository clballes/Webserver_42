/*                                                                            */
/* CGI.cpp                                                                    */
/* mpuig-ma <mpuig-ma@student.42barcelona.com>                                */
/* Tue Apr  9 16:21:17 2024                                                   */

#include "CGI.hpp"

#define READ 0
#define WRITE 1

CGI::CGI ( HTTP & http , Server & server):
	_http( http ), _server ( server ), _env( 0x0 )
{
	DEBUG( "" );
	setmap();
	map_to_arr();
	return ;
}

CGI::~CGI ( void )
{
	DEBUG( "" );
	if ( this->_env != 0x0 )
	{
		for ( std::size_t i = 0; this->_env[i] != 0x0; ++i )
			delete ( this->_env[i] );
		delete ( this->_env );
	}
	return ;
}

int
CGI::register_process ( pid_t pid )
{
    struct kevent ev;
	static struct timespec timeout = { 1, 0 };

	DEBUG( pid );
	EV_SET( &ev, pid, EVFILT_PROC,
           EV_ADD | EV_ENABLE | EV_ONESHOT,
		   NOTE_EXIT | NOTE_EXITSTATUS, 0, (void *) this );
    if ( ::kevent( IEvent::kq, &ev, 1, 0x0, 0, &timeout ) == -1 )
	{
		ERROR( PROGRAM_NAME << ": kevent: " << std::strerror( errno ) );
        return ( EXIT_FAILURE );
    }
    /*
	EV_SET( &ev, 1, EVFILT_TIMER, EV_ADD | EV_ENABLE,
			NOTE_SECONDS | NOTE_ABSOLUTE, 1, (void *) this );
    if ( ::kevent( IEvent::kq, &ev, 1, 0x0, 0, NULL ) == -1 )
	{
		ERROR( PROGRAM_NAME << ": kevent: " << std::strerror( errno ) );
        return ( EXIT_FAILURE );
    }
	*/
    return ( EXIT_SUCCESS );
}

void
CGI::dispatch ( struct kevent & ev )
{
	DEBUG( ev.ident );
	if ( ev.filter == EVFILT_PROC )
	{
		// TODO: fix
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
	else if ( ev.filter == EVFILT_TIMER )
	{
		WARN( "else" );
	}
	else
		WARN ( "HM" );
	return ;
}

// TODO: timeout

int
CGI::execute ( void )
{
	int			fdopen;
	t_request &	request = this->_http.getRequest();
	std::string	cgi;

	cgi = this->_server.getRoute( request.target ).getCGIpass();
	DEBUG( "cgi_pass=\"" << cgi << "\"" );
	DEBUG( "file=\"" << request.file << "\"" );
    fdopen = open( request.file.c_str(), O_RDONLY );
    if ( fdopen == -1 )
	{
        ERROR( request.file << ": " << std::strerror( errno ) );
		return ( EXIT_FAILURE );
    }
    if ( pipe( _pipefd ) == -1 )
	{
        ERROR( "pipe: " << strerror( errno ) );
		return ( EXIT_FAILURE );
    }
    this->_pid = fork();
	if ( this->_pid == -1 )
    {
        ERROR( "fork: " << std::strerror( errno ) );
		return ( EXIT_FAILURE );
    }
    if ( this->_pid == 0 )
    {
        close( _pipefd[READ] );
		dup2( _pipefd[WRITE], STDOUT_FILENO );
        dup2( fdopen, STDIN_FILENO );
		close( _pipefd[WRITE] );
        execve(cgi.c_str() , NULL, this->_env );
		ERROR( cgi << ": " << ::strerror( errno ) );
        exit( EXIT_FAILURE );
    }
    close( _pipefd[WRITE] );
	waitpid( this->_pid, 0x0, 0x0 );
	this->register_process( this->_pid );
	return ( EXIT_SUCCESS );
}

void
CGI::parsing_headers ( std::string line )
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

void
CGI::setmap ( void )
{
	std::map< std::string, std::string > & headers = _http.getHeaders();
	
	DEBUG ("Creating the map");
	if ( headers.find( "auth-scheme" ) != headers.end()
			&& headers["auth-scheme"] != "" )
		this->_envMap["AUTH_TYPE"] = headers["authorization"];
	this->_envMap["REDIRECT_STATUS"] = "200";
	this->_envMap["GATEWAY_INTERFACE"] = "CGI/1.1";
	this->_envMap["SERVER_PROTOCOL"] = "HTTP/1.1";
	this->_envMap["SERVER_SOFTWARE"] = WEBSERV_VER;
	this->_envMap["REQUEST_METHOD"] = _http.getRequest().method->method;
	this->_envMap["QUERY_STRING"] = _http.getRequest().query;
	this->_envMap["CONTENT_TYPE"] = headers["content-type"];
	this->_envMap["CONTENT_LENGTH"] = std::to_string(_http.getRequest().body.length());
	this->_envMap["REMOTE_IDENT"] = headers["authorization"];
	this->_envMap["REMOTE_USER"] = headers["authorization"];
	// scriptname i filename mirar del tot
	this->_envMap["SCRIPT_NAME"] = _http.getRequest().file;
	this->_envMap["SCRIPT_FILENAME"] =  _http.getRequest().file;
	this->_envMap["PATH_INFO"] = _server.getCGIpass(  this->_http.getRequest().file );
	this->_envMap["PATH_TRANSLATED"] = _server.getCGIpass(  this->_http.getRequest().file );
	this->_envMap["REQUEST_URI"] =  _server.getCGIpass(  this->_http.getRequest().file ) + _http.getRequest().query;
	if ( headers.find( "host" ) != headers.end() )
		this->_envMap["SERVER_NAME"] = headers["host"];
	this->_envMap["REMOTEaddr"] = this->_envMap["SERVER_NAME"];
	this->_envMap["SERVER_PORT"] = std::to_string(_server.getPort());
}

void
CGI::map_to_arr ( void )
{
	int index;
	std::map< std::string, std::string >::const_iterator it;

    this->_env = new char * [ (this->_envMap.size() * 2 ) + 1 ];
    index = 0;
	it = this->_envMap.begin();
	while ( it != this->_envMap.end() )
	{
		std::string keyValueString = it->first + "=" + it->second;
		this->_env[index] = new char[keyValueString.size() + 1];
		std::strcpy( this->_env[index], keyValueString.c_str() );
		++index;
		++it;
	}
    this->_env[index] = NULL;
}
