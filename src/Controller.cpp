/*                                                                            */
/* Controller.cpp                                                             */
/* mpuig-ma <mpuig-ma@student.42barcelona.com>                                */
/* Wed Apr  3 12:50:43 2024                                                   */

#include "Controller.hpp"

bool is_regular_file( const std::string & filename );
std::string & trim_comments ( std::string & str, const char * comment_type );
std::string & trim_f( std::string & str, int ( *func )( int ) );
std::string & replace_f ( std::string & str, int ( *func )( int ), char c );
std::string & normalize ( std::string & line );

t_conf_opts
Controller::_opts[] =
{
	{ CONTEXT, "server", yes, "main", 0x0 },
	{ CONTEXT, "location", yes, "server,location", 0x0 },
	{ DIRECTIVE, "server_name", no, "server", &set_server_name },
	{ DIRECTIVE, "listen", no, "server", &set_listen },
	{ DIRECTIVE, "allow_methods", no, "server,location", &set_allow_methods },
	{ DIRECTIVE, "root", no, "server,location", &set_root },
	{ DIRECTIVE, "index", no, "server,location", &set_index },
	{ DIRECTIVE, "autoindex", no, "server,location", &set_autoindex },
	{ DIRECTIVE, "cgi_pass", no, "server,location", &set_cgi_pass },
	{ DIRECTIVE, "cgi_param", no, "server,location", &set_cgi_param },
	{ DIRECTIVE, "error_page", yes, "server", &set_error_page },
	{ DIRECTIVE, "client_body", no, "server", &set_client_body },
	{ 0, 0x0, 0, 0x0, 0x0 }
};

Controller::Controller ( void ): _good( true )
{
	// Create a new kernel event queue.
	IEvent::kq = ::kqueue();
	if ( IEvent::kq == -1 )
	{
		ERROR( PROGRAM_NAME << ": kqueue: " << ::strerror( errno ) );
		this->_good = false;
	}
	DEBUG( IEvent::kq );
	return ;
}

Controller::~Controller ( void )
{
	return ;
}

bool
Controller::good ( void ) const
{
	return ( this->_good );
}

int
Controller::load ( std::string & filename )
{
	std::size_t   line_nb;
	std::string   line;
	std::ifstream file;

	DEBUG( filename.c_str() );
	file.open( filename );
	line_nb = 0;
	while ( file.good() == true && file.eof() == false )
	{
		std::getline( file, line );
		if ( normalize( line ).empty() == false )
			this->_buffer.append( line ).append( " " );
		++line_nb;
	}
	if ( file.good() == false && file.eof() != true )
		return ( EXIT_FAILURE );
	trim_f( this->_buffer, &std::isspace );
	DEBUG( this->_buffer.c_str() );
	file.close();
	setup_instances();
	return ( EXIT_SUCCESS );
}

int
Controller::load ( const char * conf_filename )
{
	std::string filename;

	if ( conf_filename == 0x0 )
		return ( EXIT_FAILURE );
	filename.assign( conf_filename );
	return ( this->load( filename ) );
}

std::size_t
how_many_options_are_there ( const t_conf_opts * opts )
{
	std::size_t n;

	n = 0;
	while ( opts != nullptr && opts[n].identifier != 0x0 )
		++n;
	return ( n );
}

std::size_t
how_many_words( std::string & str )
{
	std::string::iterator it;
	std::size_t n = 1;

	if ( str.empty() )
		return ( 0 );
	it = str.begin();
	while ( it != str.end() )
	{
		if ( std::isspace( *it )
				&& it != str.begin()
				&& std::isspace( *( it - 1 ) ) == 0)
			++n;
		++it;
	}
	return ( n );
}

std::size_t
get_directive_length ( std::string & buffer, std::size_t position )
{
	std::size_t len;

	len = 0;
	while ( position < buffer.length() && std::isspace( buffer[position] ) )
	{
		++len;
		++position;
	}
	while ( position < buffer.length() )
	{
		if ( buffer[position] == '{' || buffer[position] == '}' || buffer[position] == ';' )
		{
			++len;
			++position;
			break ;
		}
		++len;
		++position;
	}
	while ( position < buffer.length() && std::isspace( buffer[position] ) )
	{
		++len;
		++position;
	}
	return ( len != 0 ? len : 1 );
}

int
validate_directive( std::string & directive, const t_conf_opts * opts,
		bool log )
{
	const std::size_t opts_len = how_many_options_are_there( opts );
	std::string word;
	std::size_t i;

	i = 0;
	word = directive.substr( 0, directive.find_first_of( " \f\n\r\t\v{};" ) );
	while ( i < opts_len && word.compare( opts[i].identifier ) != 0 )
		++i;
	if ( i == opts_len && log == true )
	{
		ERROR( "unknown directive \"" << word.c_str() << "\"" );
		return ( EXIT_FAILURE );
	}
	if ( opts[i].type == DIRECTIVE && directive.back() != ';' && log == true )
	{
		ERROR( "directive \"" << opts[i].identifier << "\" "
				<< "is not terminated by \";\"" );
		return ( EXIT_FAILURE );
	}
	return ( EXIT_SUCCESS );
}

#include <stack>

int
Controller::setup_instances ( void )
{
	//TODO: ERROR when directive not terminated with ;
	std::size_t pos, directive_len, i;
	std::string	directive, word, argument;
	std::stack< std::string > context;
	Server * current_instance;

	pos = 0;
	context.push( "main" );
	while ( pos < this->_buffer.length() )
	{
		directive_len = get_directive_length( this->_buffer, pos );
		directive = this->_buffer.substr( pos, directive_len );
		trim_f( directive, &std::isspace );
		word = directive.substr( 0, directive.find_first_of( " \f\n\r\t\v{};" ) );
		DEBUG( directive.c_str() );
		pos += directive_len;
		if ( directive == "}" )
		{
			if ( context.size() == 1 )
			{
				ERROR( "error: context: }" );
				return ( EXIT_FAILURE );
			}
			context.pop();
			continue ;
		}
		else if ( validate_directive( directive, this->_opts, true ) )
			return ( EXIT_FAILURE );
		else if ( directive.back() == '{' )
		{
			context.push( word );
			if ( word == "server" )
			{
				this->_instances.resize( this->_instances.size() + 1 );
				current_instance = &this->_instances.back();
				LOG( "current_instance: " << current_instance->id() );
			}
			else if ( word == "location" )
			{
				argument = directive.substr( word.length(),
						directive.find_first_of( " \f\n\r\t\v{};" ) + word.length() );
				trim_f( argument, &std::ispunct );
				trim_f( argument, &std::isspace );
				// current_instance->locations new location ...
			}
			continue ;
		}
		if ( context.top() == "server" )
		{
			i = 0;
			while ( i < how_many_options_are_there( Controller::_opts )
					&& word.compare( Controller::_opts[i].identifier ) != 0 )
				++i;
			if ( i == how_many_options_are_there( Controller::_opts ) )
				return ( EXIT_FAILURE );
			std::string arg;
			arg = directive.substr( word.length(), std::string::npos );
			if ( arg.back() == ';' )
				arg.erase( arg.length() -1, 1 );
			trim_f( arg, &std::isspace );
			if ( Controller::_opts[i].set_func( *current_instance, arg ) )
				return ( EXIT_FAILURE );

		}
		else if ( context.top() == "location" )
		{
			i = 0;
			while ( i < how_many_options_are_there( Controller::_opts )
					&& word.compare( Controller::_opts[i].identifier ) != 0 )
				++i;
			if ( i == how_many_options_are_there( Controller::_opts ) )
				return ( EXIT_FAILURE );

			std::clog << "set (" << argument << ") " << word << '\n';
		}
		else
		{
			std::cout << "else\n";
		}

	}
	return ( EXIT_SUCCESS );
}

int
Controller::start ( void )
{
	if ( this->good() == false )
	{
		ERROR( PROGRAM_NAME << ": faulty controller" );
		return ( EXIT_FAILURE );
	}
	this->_instances.shrink_to_fit();
	for ( Controller::iterator it = this->_instances.begin();
			it != this->_instances.end(); ++it )
	{
		if ( it->good() == false )
		{
			ERROR( PROGRAM_NAME << ": faulty server (" << it->id() << ")" );
			return ( EXIT_FAILURE );
		}
		if ( it->start() == EXIT_FAILURE )
			return ( EXIT_FAILURE );
	}
	return ( event_loop() );
}

int
Controller::event_loop ( void )
{
	int n_events = 1;
	struct kevent ev;
	IEvent * instance;

	// TODO: graceful_stop()
	// Configure SIGINT ( signal interrupt )
	// so as to finish connections and end the program gracefully.
	//::signal( SIGINT, &graceful_stop );
	//::signal( SIGQUIT, &graceful_stop );
	DEBUG( IEvent::kq );
	while ( this->good() )
	{
		n_events = ::kevent( IEvent::kq, 0x0, 0, &ev, 1, 0 );
		if ( this->good() == false )
			return ( EXIT_FAILURE );
		if ( n_events == -1 )
		{
			ERROR( "kevent: " << ::strerror( errno ) );
			break ;
		}
		else if ( n_events == 0 )
			continue ;
		LOG( "(ev=" << ev.ident << ")" );
		instance = static_cast< IEvent * >( ev.udata );
		instance->dispatch( ev );
		// consider EVFILT_SIGNAL
	}
	LOG( "close kq (fd=" << IEvent::kq << ")" );
	close( IEvent::kq );
	return ( EXIT_SUCCESS );
}

const std::vector< Server > &
Controller::getInstances ( void ) const
{
	DEBUG( this );
	return ( this->_instances );
}

#include <sys/stat.h>

bool
is_regular_file( const std::string & filename )
{
	struct stat file_info;

	DEBUG( filename.c_str() );
    if ( stat( filename.c_str(), &file_info ) != 0 )
        return ( false );
    return ( S_ISREG( file_info.st_mode ) );
}

std::string &
trim_comments ( std::string & str, const char * comment_type )
{
	size_t pos;

	if ( comment_type == 0x0 )
		return ( str );
	pos = str.find_first_of( comment_type, 0 );
	if ( pos != std::string::npos )
		str = str.erase( pos, str.length() - pos );
	return ( str );
}

std::string &
trim_f( std::string & str, int ( *func )( int ) )
{
	std::string::iterator it;

	if ( str.empty() )
		return ( str );
	it = str.end() - 1;
	while ( it != str.begin() && func( *it ) )
		str.erase( it-- );
	it = str.begin();
	while ( it != str.end() && func( *it ) )
		it = str.erase ( it );
	return ( str );
}

std::string &
replace_f ( std::string & str, int ( *func )( int ), char c )
{
	std::string::iterator it;

	it = str.begin();
	while ( it != str.end() )
	{
		if ( func( *it ) != 0 )
			*it = c;
		++it;
	}
	return ( str );
}

std::string &
del_multipl_sp( std::string & str, int ( *func )( int ) )
{
	std::string::iterator it;
	bool prev_sp;

	it = str.begin();
	prev_sp = (bool) func( *it );
	while ( it != str.end() )
	{
		if ( prev_sp == true && func( *it ) != 0 )
		{
			it = str.erase( it );
			prev_sp = (bool) func( *it );
		}
		else
		{
			prev_sp = (bool) func( *it );
			++it;
		}
	}
	return ( str );
}

std::string &
normalize ( std::string & line )
{
	line = trim_comments( line, "#" );
	line = trim_f( line, &std::isspace );
	line = replace_f( line, &std::isspace, ' ' );
	line = del_multipl_sp( line, &std::isspace );
	return ( line );
}


int
set_allow_methods( Server & instance, std::string & arg )
{
	//TODO: implement t_methods from HTTP
	//TODO: return EXIT_FAILURE if setFlag returns EXIT_FAILURE
	std::istringstream iss( arg );
	std::string word;

	DEBUG( arg.c_str() );
	if ( arg.empty() )
	{
		ERROR( "invalid number of arguments in \"allow_methods\"" );
		return ( EXIT_FAILURE );
	}
	while ( iss >> word )
	{
		if ( word == "GET" )
			instance.getConf().setFlag( METHOD_GET, true );
		else if ( word == "PUT" )
			instance.getConf().setFlag( METHOD_PUT, true );
		else if ( word == "POST" )
			instance.getConf().setFlag( METHOD_POST, true );
		else if ( word == "HEAD" )
			instance.getConf().setFlag( METHOD_HEAD, true );
		else
		{
			ERROR( "invalid method \"" << word << "\"" );
			return ( EXIT_FAILURE );
		}
	}
	return ( EXIT_SUCCESS );
}

int
set_autoindex( Server & instance, std::string & arg )
{
	DEBUG( arg.c_str() );
	if ( arg.empty() )
		return ( EXIT_FAILURE );
	else if ( arg == "off" )
		return ( instance.getConf().setFlag( F_AUTOINDEX, false ) );
	else if ( arg == "on" )
		return ( instance.getConf().setFlag( F_AUTOINDEX, true ) );
	else
		ERROR( "not a valid value for \"autoindex\"" );
	return ( EXIT_FAILURE );
}

int
set_cgi_param ( Server & instance, std::string & arg )
{
	//TODO: what if a path has spaces ???
	DEBUG( arg.c_str() );
	if ( arg.empty() || arg.find( " " ) != std::string::npos )
	{
		ERROR( "invalid number of arguments in \"cgi_pass\"" );
		return ( EXIT_FAILURE );
	}
	return ( instance.getConf().setCGIparam( arg ) );
}

int
set_cgi_pass ( Server & instance, std::string & arg )
{
	//TODO: what if a path has spaces ???
	DEBUG( arg.c_str() );
	if ( arg.empty() || arg.find( " " ) != std::string::npos )
	{
		ERROR( "invalid number of arguments in \"cgi_pass\"" );
		return ( EXIT_FAILURE );
	}
	return ( instance.getConf().setCGIpass( arg ) );
}

int
set_client_body ( Server & instance, std::string & arg )
{
	//TODO: multiply value for 'M, m, K, k'
	std::size_t n = 0;
	int alpha = 0;

	DEBUG( arg.c_str() );
	if ( arg.empty() || arg.find( " " ) != std::string::npos )
	{
		ERROR( "invalid number of arguments in \"client_body\"" );
		return ( EXIT_FAILURE );
	}
    for ( std::size_t i = 0; i < arg.length(); i++ )
    {
        if ( ! isdigit( arg[i] )
				&& arg[i] != 'm' && arg[i] != 'M'
				&& arg[i] != 'k' && arg[i] != 'K' )
			return ( EXIT_FAILURE );
		if ( isalpha( arg[i] ) )
			alpha = arg[i];
		if ( alpha != 0 && i != arg.length() - 1 )
			return ( EXIT_FAILURE );
    }
	if ( alpha != 0 )
		arg.erase( arg.length() - 1, 1 );
    n = static_cast<std::size_t>( std::atoi( arg.c_str() ) );
	if ( alpha == 'M' || alpha == 'm' )
		n *= 1000;
	return ( instance.getConf().setClientMaxBodySize( n ) );
}

int
set_error_page ( Server & instance, std::string & arg )
{
	//TODO: expects 2 arguments only
	std::istringstream iss( arg );
	std::string::iterator it;
	std::string num, page;

	DEBUG( arg.c_str() );
	if ( arg.empty() || how_many_words( arg ) != 2 )
	{
		ERROR( "invalid number of arguments in \"error_page\"" );
		return ( EXIT_FAILURE );
	}
	iss >> num;
	for ( std::string::iterator it = num.begin(); it != num.end(); ++it )
	{
		if ( std::isdigit( *it ) == 0 )
		{
			ERROR( "invalid argument in \"error_page\"" );
			return ( EXIT_FAILURE );
		}
	}
	iss >> page;
	if ( instance.getConf().setErrorPage( std::atoi( num.c_str() ), page ) )
		return ( EXIT_FAILURE );
	return ( EXIT_SUCCESS );
}

int
set_index ( Server & instance, std::string & arg )
{
	std::istringstream iss( arg );
	std::string word;

	DEBUG( arg.c_str() );
	if ( arg.empty() )
	{
		ERROR( "invalid number of arguments in \"index\"" );
		return ( EXIT_FAILURE );
	}
	while ( iss >> word )
	{
		if ( instance.getConf().setIndex( word ) == EXIT_FAILURE )
			return ( EXIT_FAILURE );
	}
	return ( EXIT_SUCCESS );
}

int
set_listen( Server & instance, std::string & arg )
{
	struct sockaddr_in * address = nullptr;
	struct addrinfo hints, * result, * rp;
	std::istringstream iss( arg );
	std::string ip, port;
	int ecode;

	DEBUG( arg.c_str() );
	if ( arg.empty() || arg.find( " " ) != std::string::npos )
	{
		ERROR( "invalid number of arguments in \"listen\"" );
		return ( EXIT_FAILURE );
	}
	ip.assign( "0.0.0.0" );
	port.assign( "80" );
	if ( arg.find( ':' ) != std::string::npos )
	{
		std::getline( iss, ip, ':' );
		std::getline( iss, port );
	}
	else if ( arg.find( '.' ) != std::string::npos )
		std::getline( iss, ip );
	else
		std::getline( iss, port );
	std::memset( &hints, 0, sizeof( hints ) );
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;
	ecode = getaddrinfo( ip.c_str(), port.c_str(), &hints, &result );
	if ( ecode != 0 )
	{
		ERROR( gai_strerror( ecode ) );
		return ( EXIT_FAILURE );
	}
	rp = result;
	while ( rp != nullptr )
	{
		if ( rp->ai_family == AF_INET )
		{
			address = (struct sockaddr_in * ) rp->ai_addr;
			break ;
		}
		rp = rp->ai_next;
	}
	if ( address == nullptr )
		return ( EXIT_FAILURE );
	ecode = instance.setListen( *address );
	freeaddrinfo( result );
	if ( ecode == EXIT_FAILURE )
		return ( EXIT_FAILURE );
	return ( EXIT_SUCCESS );
}

int
set_root ( Server & instance, std::string & arg )
{
	DEBUG( arg.c_str() );
	if ( arg.empty() || arg.find( " " ) != std::string::npos )
	{
		ERROR( "invalid number of arguments in \"root\"" );
		return ( EXIT_FAILURE );
	}
	return ( instance.getConf().setRoot( arg ) );
}

int
set_server_name ( Server & instance, std::string & arg )
{
	std::istringstream iss( arg );
	std::string word;

	DEBUG( arg.c_str() );
	if ( arg.empty() )
	{
		ERROR( "invalid number of arguments in \"server_name\"" );
		return ( EXIT_FAILURE );
	}
	while ( iss >> word )
	{
		if ( instance.getConf().setServerName( word ) )
			return ( EXIT_FAILURE );
	}
	return ( EXIT_SUCCESS );
}
