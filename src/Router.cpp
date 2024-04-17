/*                                                                            */
/* Router.cpp                                                                 */
/* mpuig-ma <mpuig-ma@student.42barcelona.com>                                */
/* Tue Apr 16 11:52:05 2024                                                   */

#include "Router.hpp"

t_conf_opts
Router::_opts[] =
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

Router::Router ( void ): _good( true )
{
	IEvent::kq = ::kqueue();
	DEBUG ( IEvent::kq );
	if ( IEvent::kq == -1 )
	{
		ERROR( "kqueue: " << ::strerror( errno ) );
		this->_good = false;
	}
	return ;
}

Router::~Router ( void )
{
	return ;
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

t_conf_opts *
get_option ( std::string & opt_name, const t_conf_opts * opts )
{
	DEBUG( opt_name );
	const std::size_t	opts_len = how_many_options_are_there( opts );
	std::size_t			i;

	i = 0;
	while ( i < opts_len && opt_name.compare( opts[i].identifier ) != 0 )
		++i;
	return ( (t_conf_opts * ) &opts[i] );
}

std::size_t
get_directive_length ( std::string & buffer, std::size_t position )
{
	const std::size_t init_position = position;

	while ( position < buffer.length() && std::isspace( buffer[position] ) )
		++position;
	while ( position < buffer.length() )
	{
		if ( buffer[position] == '}' )
		{
			if ( position - init_position == 0 )
				++position;
			break ;
		}
		if ( buffer[position] == ';' || buffer[position] == '{' )
		{
			++position;
			break ;
		}
		++position;
	}
	while ( position < buffer.length() && std::isspace( buffer[position] ) )
		++position;
	return ( position - init_position );
}

int
validate_directive ( std::string & directive, const t_conf_opts * opts )
{
	const std::size_t	opts_len = how_many_options_are_there( opts );
	std::string			word;
	std::size_t			i;

	i = 0;
	if ( directive == "}" )
		return ( EXIT_SUCCESS );
	word = directive.substr( 0, directive.find_first_of( " \f\n\r\t\v;{}" ) );
	while ( i < opts_len && word.compare( opts[i].identifier ) != 0 )
		++i;
	if ( i == opts_len )
	{
		ERROR( "unknown directive \"" << word << "\"" );
		return ( EXIT_FAILURE );
	}
	if ( opts[i].type == DIRECTIVE && directive.back() != ';' )
	{
		ERROR( "directive \"" << opts[i].identifier << "\" "
				<< "is not terminated with \";\"" );
		return ( EXIT_FAILURE );
	}
	if ( opts[i].type == CONTEXT && directive.back() != '{' )
	{
		ERROR( "context \"" << opts[i].identifier << "\" "
				<< "is not terminated with \"{\"" );
		return ( EXIT_FAILURE );
	}
	return ( EXIT_SUCCESS );
}

int
Router::load ( std::string filename )
{
	std::string   buffer, line;
	std::ifstream file;

	DEBUG( filename.c_str() );
	if ( is_regular_file( filename ) == false )
	{
		ERROR( filename << ": Is a directory" );
		return ( EXIT_FAILURE );
	}
	file.open( filename );
	while ( file.good() == true && file.eof() == false )
	{
		std::getline( file, line );
		if ( normalize( line ).empty() == false )
			buffer.append( line ).append( " " );
	}
	if ( file.good() == false && file.eof() != true )
	{
		ERROR( filename << ": " << strerror( errno ) );
		return ( EXIT_FAILURE );
	}
	trim_f( buffer, &std::isspace );
	file.close();
	if ( parse( buffer ) == EXIT_FAILURE )
	{
		this->_good = false;
		return ( EXIT_FAILURE );
	}
	return ( EXIT_SUCCESS );
}

int
Router::parse( std::string & buffer )
{
	std::stack< std::string >	context;
	std::string					directive, directive_name, directive_value;
	std::string					location;
	std::size_t					directive_len, position = 0;
	Location *					current_route = nullptr;

	while ( position < buffer.length() )
	{
		directive_len = get_directive_length( buffer, position );
		directive = buffer.substr( position, directive_len );
		position += directive_len;
		trim_f( directive, &std::isspace );
		directive_name = get_word( directive, " \f\n\r\t\v;{}" );
		if ( validate_directive( directive, this->_opts ) == EXIT_FAILURE )
			return ( EXIT_FAILURE );
		DEBUG( directive );
		DEBUG( directive_name );
		if ( context.empty() && directive == "}" )
		{
			ERROR( "error: context: }" );
			return ( EXIT_FAILURE );
		}
		else if ( directive.back() == '{' )
		{
			context.push( directive_name );
			if ( context.top() == "server" )
			{
				//this->_servers.resize( this->_servers.size() + 1 );
				this->_servers.push_back( Server() );
				//current_server = &this->_servers.back();
			}
			else if ( context.top() == "location" )
			{
				directive_value = directive.substr( directive_name.length()	);
				directive_value = directive_value.substr( 0,
						directive_value.find_first_of( "{}" ) );
				trim_f( directive_value, &std::isspace );
				if ( directive_value.empty() )
				{
					ERROR( "unnamed \"location\"." );
					return ( EXIT_FAILURE );
				}
				(void) current_route;
				// TODO: check only one argument
				//if ( current_route == nullptr )
				//	current_route = &current_server->getDefaultRoute();
				//else
				//	current_route = &current_server->getRoute( directive_value );
				directive_value.clear();
			}
			continue ;
		}
		else if ( directive == "}" )
		{
			context.pop();
			continue ;
		}
		else if ( context.top() == "server" )
		{
			directive_value = directive.substr( directive_name.length() );
			//		directive.find_first_of( ";" ) - directive_name.length() );
			if ( directive_value.empty() == false && directive_value.back() == ';' )
				directive_value.erase( directive_value.length() - 1, 1 );
			trim_f( directive_value, &std::isspace );
			t_conf_opts * opt = get_option( directive_name, this->_opts );
			if ( opt->set_func( this->_servers.back(), directive_value, location ) )
				return ( EXIT_FAILURE );
			directive_value.clear();
		}
		else if ( context.top() == "location" )
		{
		}
	}
	return ( EXIT_SUCCESS );
}

int
Router::listen ( void )
{
	int n_events = 1;
	struct kevent ev;
	IEvent * instance;

	DEBUG( IEvent::kq );
	for ( std::vector< Server >::iterator i = this->_servers.begin();
			i != this->_servers.end(); ++i )
	{
		if ( this->setConnection( i->getListen() ) == EXIT_FAILURE )
			return ( EXIT_FAILURE );
	}
	if ( this->_connections.empty() )
	{
		INFO( PROGRAM_NAME << ": there are no connections to listen to." );
		return ( EXIT_SUCCESS );
	}
	for ( std::vector< Connection >::iterator i = this->_connections.begin();
			i != this->_connections.end(); ++i )
	{
		if ( this->register_read_socket( *i ) == EXIT_FAILURE )
			return ( EXIT_FAILURE );
	}
	// TODO: graceful_stop(), SIGINT
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
		instance = static_cast< IEvent * >( ev.udata );
		instance->dispatch( ev );
		// TODO: consider EVFILT_SIGNAL
	}
	return ( EXIT_SUCCESS );
}

int
Router::register_read_socket ( Connection & instance ) const
{
	struct kevent ev;
	//static struct timespec ev_timeout;

	DEBUG( "fd=" << instance.getSocketFD() );
	EV_SET( &ev, instance.getSocketFD(), EVFILT_READ,
			EV_ADD | EV_ENABLE | EV_CLEAR, 0, 0, (void *) this );
	if ( ::kevent( IEvent::kq, &ev, 1, 0x0, 0, 0 ) == -1 )
	{
		ERROR( "kevent: " << ::strerror( errno ) );
		return ( EXIT_FAILURE );
	}
	return ( EXIT_SUCCESS );
}

void
Router::dispatch ( struct kevent & ev )
{
	HTTP * client;

	DEBUG ( "ev=" << ev.ident );
	if ( ev.filter != EVFILT_READ )
	{
		WARN( "unknown event filter" );
		return ;
	}
	client = new HTTP( *this, ev.ident );
	return ;
}

bool
Router::good ( void ) const
{
	return ( this->_good );
}

Server *
Router::getServer ( std::string & server_name )
{
	DEBUG( server_name );
	if ( this->_servers.empty() )
		return ( nullptr );
	return ( &this->_servers[0] );
}

int
Router::setConnection ( const struct sockaddr_in & address,
		int domain, int type, int protocol )
{
	this->_connections.push_back( Connection( address,
				domain, type, protocol ) );
	return ( EXIT_SUCCESS );
}

int
set_allow_methods( Server & instance, std::string & arg, std::string location )
{
	//TODO: implement t_methods from HTTP
	//TODO: return EXIT_FAILURE if setFlag returns EXIT_FAILURE
	std::istringstream iss( arg );
	std::string word;
	
	(void) location;
	DEBUG( arg );
	if ( arg.empty() )
	{
		ERROR( "invalid number of arguments in \"allow_methods\"" );
		return ( EXIT_FAILURE );
	}
	while ( iss >> word )
	{
		if ( word == "GET" )
			instance.setFlag( METHOD_GET, true, location );
		else if ( word == "PUT" )
			instance.setFlag( METHOD_PUT, true, location );
		else if ( word == "POST" )
			instance.setFlag( METHOD_POST, true, location );
		else if ( word == "HEAD" )
			instance.setFlag( METHOD_HEAD, true, location );
		else
		{
			ERROR( "invalid method \"" << word << "\"" );
			return ( EXIT_FAILURE );
		}
	}
	return ( EXIT_SUCCESS );
}

int
set_autoindex( Server & instance, std::string & arg, std::string location )
{
	(void) location;
	DEBUG( arg );
	if ( arg.empty() )
		return ( EXIT_FAILURE );
	else if ( arg == "off" )
		return ( instance.setFlag( F_AUTOINDEX, false ) );
	else if ( arg == "on" )
		return ( instance.setFlag( F_AUTOINDEX, true ) );
	else
		ERROR( "not a valid value for \"autoindex\"" );
	return ( EXIT_FAILURE );
}

int
set_cgi_param ( Server & instance, std::string & arg, std::string location )
{
	(void) location;
	//TODO: what if a path has spaces ???
	DEBUG( arg );
	if ( arg.empty() || arg.find( " " ) != std::string::npos )
	{
		ERROR( "invalid number of arguments in \"cgi_pass\"" );
		return ( EXIT_FAILURE );
	}
	return ( instance.setCGIparam( arg ) );
}

int
set_cgi_pass ( Server & instance, std::string & arg, std::string location )
{(void) location;
	//TODO: what if a path has spaces ???
	DEBUG( arg );
	if ( arg.empty() || arg.find( " " ) != std::string::npos )
	{
		ERROR( "invalid number of arguments in \"cgi_pass\"" );
		return ( EXIT_FAILURE );
	}
	return ( instance.setCGIpass( arg ) );
}

int
set_client_body ( Server & instance, std::string & arg, std::string location )
{(void) location;
	//TODO: multiply value for 'M, m, K, k'
	std::size_t n = 0;
	int alpha = 0;

	DEBUG( arg );
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
	return ( instance.setClientMaxBodySize( n ) );
}

int
set_error_page ( Server & instance, std::string & arg, std::string location )
{(void) location;
	//TODO: expects 2 arguments only
	std::istringstream iss( arg );
	std::string::iterator it;
	std::string num, page;

	DEBUG( arg );
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
	if ( instance.setErrorPage( std::atoi( num.c_str() ), page ) )
		return ( EXIT_FAILURE );
	return ( EXIT_SUCCESS );
}


int
set_index ( Server & instance, std::string & arg, std::string location )
{(void) location;
	DEBUG( arg );
	(void) arg;
	(void) instance;
	return ( 0 );
};

int
set_listen( Server & instance, std::string & arg, std::string location )
{(void) location;
	struct sockaddr_in * address = nullptr;
	struct addrinfo hints, * result, * rp;
	std::istringstream iss( arg );
	std::string ip, port;
	int ecode;
   
	DEBUG( arg );
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
set_root ( Server & instance, std::string & arg, std::string location )
{(void) location;
	DEBUG( arg );
	if ( arg.empty() || arg.find( " " ) != std::string::npos )
	{
		ERROR( "invalid number of arguments in \"root\"" );
		return ( EXIT_FAILURE );
	}
	instance.setRoot( arg );
	return ( EXIT_SUCCESS );
}

int
set_server_name ( Server & instance, std::string & arg, std::string location )
{(void) location;
	std::istringstream iss( arg );
	std::string word;
   
	DEBUG( arg );
	if ( arg.empty() )
	{
		ERROR( "invalid number of arguments in \"server_name\"" );
		return ( EXIT_FAILURE );
	}
	while ( iss >> word )
	{
		if ( instance.setServerName( word ) )
			return ( EXIT_FAILURE );
	}
	return ( EXIT_SUCCESS );
}
