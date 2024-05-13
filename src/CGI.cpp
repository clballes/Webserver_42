/*                                                                            */
/* CGI.cpp                                                                    */
/* mpuig-ma <mpuig-ma@student.42barcelona.com>                                */
/* Tue Apr  9 16:21:17 2024                                                   */

#include "CGI.hpp"

#define READ 0
#define WRITE 1

std::size_t
CGI::timer_id_threshold = INT_MAX;

static char ** map2array ( const std::map< std::string, std::string > & );

CGI::CGI ( HTTP & http , Server & server):
	_http( http ), _server ( server )
{
	DEBUG( "" );
	this->_timer_id = CGI::timer_id_threshold++;
	this->setmap();
	return ;
}

CGI::~CGI ( void )
{
	DEBUG( "" );
	return ;
}

void
CGI::setmap ( void )
{
	std::map< std::string, std::string > & headers = _http.getHeaders( 0 );
	const t_request & request = this->_http.getRequest();
	const std::string & cgi_pass = this->_server.getCGIpass( request.target );
	
	DEBUG ( "" );
	if ( headers.find( "auth-scheme" ) != headers.end()
			&& headers["auth-scheme"] != "" )
		this->_envMap["AUTH_TYPE"] = headers["authorization"];
	this->_envMap["REDIRECT_STATUS"] = "200";
	this->_envMap["GATEWAY_INTERFACE"] = "CGI/1.1";
	this->_envMap["SERVER_PROTOCOL"] = "HTTP/1.1";
	this->_envMap["SERVER_SOFTWARE"] = WEBSERV_VER;
	this->_envMap["REQUEST_METHOD"] = request.method->method;
	this->_envMap["QUERY_STRING"] = request.query;
	this->_envMap["CONTENT_TYPE"] = headers["content-type"];
	if ( !request.body.empty() )
		this->_envMap["CONTENT_LENGTH"] = my_to_string( request.body.length() );
	this->_envMap["PATH_INFO"] = cgi_pass;
	this->_envMap["PATH_TRANSLATED"] = cgi_pass;
	this->_envMap["REQUEST_URI"] =  cgi_pass + request.query;
	if ( headers.find( "host" ) != headers.end() )
		this->_envMap["SERVER_NAME"] = headers["host"];
	this->_envMap["REMOTEaddr"] = this->_envMap["SERVER_NAME"];
	this->_envMap["SERVER_PORT"] = my_to_string( _server.getPort() );
	return ;
}

int
CGI::register_process ( void )
{
    struct kevent ev;

	DEBUG( this->_pid );
	EV_SET( &ev, this->_pid, EVFILT_PROC, EV_ADD | EV_ENABLE | EV_ONESHOT,
		   NOTE_EXIT | NOTE_EXITSTATUS, 0, (void *) this );
    if ( ::kevent( IEvent::kq, &ev, 1, 0x0, 0, 0x0 ) == -1 )
	{
		ERROR( "kevent: " << std::strerror( errno ) );
        return ( EXIT_FAILURE );
    }
	EV_SET( &ev, this->_timer_id, EVFILT_TIMER, EV_ADD | EV_ENABLE,
			NOTE_SECONDS, CGI_TIMEOUT, (void *) this );
    if ( ::kevent( IEvent::kq, &ev, 1, 0x0, 0, 0x0 ) == -1 )
	{
		ERROR( PROGRAM_NAME << ": kevent: " << std::strerror( errno ) );
        return ( EXIT_FAILURE );
    }
    return ( EXIT_SUCCESS );
}

int
CGI::deregister_process( void )
{
	struct kevent ev;

	DEBUG ( this->_pid );
	EV_SET( &ev, this->_pid, EVFILT_PROC, EV_DISABLE | EV_DELETE, 0, 0, 0x0 );
	if ( ::kevent( IEvent::kq, &ev, 1, 0x0, 0, NULL ) == -1 )
	{
		ERROR( PROGRAM_NAME << ": kevent: " << std::strerror( errno ) );
        return ( EXIT_FAILURE );
    }
	return ( EXIT_SUCCESS );
}

int
CGI::deregister_timer ( void )
{
	struct kevent ev;

	DEBUG ( this->_pid );
	EV_SET( &ev, this->_timer_id, EVFILT_TIMER, EV_DISABLE | EV_DELETE, 0, 0, 0x0 );
	if ( ::kevent( IEvent::kq, &ev, 1, 0x0, 0, NULL ) == -1 )
	{
		ERROR( PROGRAM_NAME << ": kevent: " << std::strerror( errno ) );
        return ( EXIT_FAILURE );
    }
	CGI::timer_id_threshold -= 1;
	return ( EXIT_SUCCESS );
}

void
CGI::dispatch ( struct kevent & ev )
{
	std::string buffer;
	int bytes_read;

	DEBUG( ev.ident );
	if ( ev.filter == EVFILT_PROC )
	{
		this->deregister_timer();
		char buf[1024];

		bytes_read = read( this->_pipefd[READ], buf, sizeof( buf ) );
		while ( bytes_read > 0 )
		{
			buffer.append( buf, bytes_read );
			bytes_read = read( this->_pipefd[READ], buf, sizeof( buf ) );
		}
		if ( bytes_read == -1 )
		{
			ERROR( "CGI: " << strerror( errno ) );
			this->_http.setStatusCode( BAD_GATEWAY );
			this->_http.register_send();
			delete this;
			return ;
		}
		LOG_BUFFER( buffer, YELLOW );
		close( this->_pipefd[READ] );
		if ( parse_headers( buffer ) == EXIT_FAILURE )
		{
			this->_http.setStatusCode( BAD_GATEWAY );
		}
		if ( check_headers() == EXIT_FAILURE )
		{
			this->_http.setStatusCode( INTERNAL_SERVER_ERROR );
		}
		this->_http.compose_response();
		delete this;
	}
	else if ( ev.filter == EVFILT_TIMER )
	{
		WARN( "time is up for this cgi" );
		this->deregister_timer();
		this->_http.setStatusCode( GATEWAY_TIMEOUT );
		this->_http.compose_response();
		this->_http.register_send();
		delete this;
	}
	return ;
}

int
CGI::execute ( void )
{
	const t_request &	request = this->_http.getRequest();
	char **				env;
	std::string			cgi;
	int					fdopen;

	env = map2array( this->_envMap );
	if ( env == NULL )
	{
		ERROR( PROGRAM_NAME << ": could not allocate memory" );
		return ( EXIT_FAILURE );
	}
	cgi = this->_server.getRoute( request.target ).getCGIpass();
	DEBUG( "cgi_pass=\"" << cgi << "\"" );
	DEBUG( "file=\"" << request.file << "\"" );
	if ( can_access_file( cgi, X_OK ) == false )
	{
		ERROR( cgi << ": " << std::strerror( errno ) );
		this->_http.setStatusCode( BAD_GATEWAY );
		return ( EXIT_FAILURE );
	}
    fdopen = open( request.file.c_str(), O_RDONLY );
    if ( fdopen == -1 )
	{
        ERROR( request.file << ": " << std::strerror( errno ) );
		this->_http.setStatusCode( NOT_FOUND );
		return ( EXIT_FAILURE );
    }
    if ( pipe( this->_pipefd ) == -1 )
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
        execve( cgi.c_str() , NULL, (char * const *) env );
		ERROR( cgi << ": " << ::strerror( errno ) );
        exit( EXIT_FAILURE );
    }
    close( _pipefd[WRITE] );
	for ( std::size_t i = 0; env[i] != 0x0; ++i )
		delete ( env[i] );
	delete ( env );
	this->register_process();
	return ( EXIT_SUCCESS );
}

void
CGI::kill ( void )
{
	if ( this->_pid != 0 )
		::kill( this->_pid, SIGKILL );
	delete this;
	return ;
}

int CGI::check_headers()
{
    std::map<std::string, std::string>& headers = _http.getHeaders(1);
    int flag = 0;
    if (headers.find("status") != headers.end()) {
        ++flag;
    }
    if (headers.find("content-type") != headers.end()) {
        ++flag;
    }
    if (headers.find("location") != headers.end()) {
        ++flag;
    }
    return (flag == 0) ? EXIT_FAILURE : EXIT_SUCCESS;
}

int
CGI::parse_headers( std::string & line )
{
	std::map <std::string, std::string> mapHeaders;
	std::string::size_type  start, pos;
	std::string             sub_line;

	start = 0;
	pos = line.find_first_of( LF, start );
	if ( pos == std::string::npos )
		pos = line.length();
	while (pos != std::string::npos)
	{
            std::string sub_line = line.substr(start, pos - start);
            // Trim leading and trailing whitespaces
            sub_line = normalize(sub_line);
			sub_line = tolower_string(sub_line);
            size_t pos_point = sub_line.find(":");
            if (pos_point != std::string::npos)
			{
                std::string str = sub_line.substr(0, pos_point);
                std::string str2;
                if (str.compare("Status") == 0) {
                    str2 = sub_line.substr(pos_point + 2, 3);
                } else {
                    str2 = sub_line.substr(pos_point + 2);
                }
                str2 = normalize(str2);
                mapHeaders[str] = str2;
            } else if (sub_line.empty() || (sub_line.length() == 1 && sub_line[0] == '\r'))
			{
				if (pos + 1 != line.length() )
				{
					std::string body = line.substr(pos +1, line.length());
					this->_http.setMessageBody( body );
				}	
				break;
            }
		start = pos + 1;
		pos = line.find_first_of( LF, start );
	}
	for ( std::map<std::string, std::string>::iterator it = mapHeaders.begin();
			it != mapHeaders.end(); ++it )
	{
		this->_http.setResponseHeaders ( it->first, it->second );
	}
	return (EXIT_SUCCESS);
}

static char **
map2array ( const std::map< std::string, std::string > & envMap )
{
	std::map< std::string, std::string >::const_iterator it;
	char ** env = NULL;
	int index = 0;

    env = new char * [ (envMap.size() * 2 ) + 1 ];
	if ( env == NULL )
		return ( NULL );
	it = envMap.begin();
	while ( it != envMap.end() )
	{
		std::string keyValueString = it->first + "=" + it->second;
		env[index] = new char[keyValueString.size() + 1];
		std::strcpy( env[index], keyValueString.c_str() );
		++index;
		++it;
	}
    env[index] = NULL;
	return ( env );
}
