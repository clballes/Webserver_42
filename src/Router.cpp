/*                                                                            */
/* Router.cpp                                                                 */
/* mpuig-ma <mpuig-ma@student.42barcelona.com>                                */
/* Tue Apr 16 11:52:05 2024                                                   */

#include "Router.hpp"

int
IEvent::kq = 0;

t_conf_opts
Router::_opts[] =
{
	// type, identifier, duplicated, required, nest, set_func
	{ CONTEXT, "server", yes, yes, "main", 0x0 },
	{ CONTEXT, "location", yes, no, "server", 0x0 },
	{ DIRECTIVE, "server_name", no, no, "server", &set_server_name },
	{ DIRECTIVE, "listen", no, yes, "server", &set_listen },
	{ DIRECTIVE, "limit_except", no, yes, "location", &set_limit_except },
	{ DIRECTIVE, "root", no, yes, "server,location", &set_root },
	{ DIRECTIVE, "index", no, no, "server,location", &set_index },
	{ DIRECTIVE, "autoindex", no, no, "server,location", &set_autoindex },
	{ DIRECTIVE, "cgi_pass", no, no, "location", &set_cgi_pass },
	// { DIRECTIVE, "cgi_param", no, no, "location", &set_cgi_param },
	{ DIRECTIVE, "error_page", yes, no, "server", &set_error_page },
	{ DIRECTIVE, "client_max_body_size", no, no, "server,location", &set_client_max_body_size },
	{ DIRECTIVE, "redirection", no, no, "location", &set_redirection },
	{ DIRECTIVE, "upload_directory", no, no, "server,location", &set_upload_directory },
	{ 0, 0x0, 0, 0, 0x0, 0x0 },
};

// TODO: make sure default server is configured
// similar to default route
Router::Router ( void ): _good( true )
{
	IEvent::kq = ::kqueue();
	if ( IEvent::kq == -1 )
	{
		ERROR( "kqueue: " << std::strerror( errno ) );
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
	while ( opts != NULL && opts[n].identifier != NULL )
		++n;
	return ( n );
}

t_conf_opts *
get_option ( std::string & opt_name, const t_conf_opts * opts )
{
	const std::size_t	opts_len = how_many_options_are_there( opts );
	std::size_t			i;

	i = 0;
	while ( i < opts_len && opt_name.compare( opts[i].identifier ) != 0 )
		++i;
	if ( i == opts_len )
		return ( NULL );
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
check_context ( std::string & directive_name, std::string & context,
		const t_conf_opts * opts )
{
	t_conf_opts * opt;
	std::string valid_contexts;

	opt = get_option( directive_name, opts );
	if ( opt == NULL )
		return ( EXIT_FAILURE );
	valid_contexts.assign( opt->nest );
	if ( valid_contexts.find( context ) == std::string::npos )
	{
		ERROR( "directive \"" << directive_name << "\" in wrong context" );
		return ( EXIT_FAILURE );
	}
	return ( EXIT_SUCCESS );
}

int
compare_servers ( std::vector< Server > & servers )
{
	std::vector< Server >::const_iterator it;
	
	if ( servers.size() <= 1 )
		return ( EXIT_SUCCESS );
	it = servers.begin() + 1;
	while ( it != servers.end() )
	{
		const Server & a = *( it - 1 );
		const Server & b = *it;
		if ( a.getPort() == b.getPort() && a.getHost() == b.getHost() )
		{
			const std::vector< std::string > & s_names = a.getServerNames();
			for ( std::vector< std::string >::const_iterator n = s_names.begin();
					n != s_names.end(); ++n )
			{
				if ( b.hasServerName( const_cast< std::string & >( *n ) ) )
				{
					ERROR( "server_name \"" << *n << "\" is repeated" );
					return ( EXIT_FAILURE );
				}
			}
		}
		++it;
	}
	return ( EXIT_SUCCESS );
}

int
Router::load ( std::string filename )
{
	std::string   buffer, line;
	std::ifstream file;

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
		ERROR( filename << ": " << std::strerror( errno ) );
		return ( EXIT_FAILURE );
	}
	trim_f( buffer, &std::isspace );
	file.close();
	if ( this->parse( buffer ) == EXIT_FAILURE )
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
	std::string					location( "" );
	std::size_t					directive_len, position = 0;

	context.push( "main" );
	while ( position < buffer.length() )
	{
		directive_len = get_directive_length( buffer, position );
		directive = buffer.substr( position, directive_len );
		position += directive_len;
		trim_f( directive, &std::isspace );
		directive_name = get_word( directive, " \f\n\r\t\v;{}" );
		if ( validate_directive( directive, this->_opts ) == EXIT_FAILURE )
			return ( EXIT_FAILURE );
		if ( directive_name.empty() == false
				&& check_context( directive_name, context.top(), this->_opts ) )
			return ( EXIT_FAILURE );
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
				this->_servers.push_back( Server() );
			}
			else if ( context.top() == "location" )
			{
				directive_value = directive.substr( directive_name.length()	);
				directive_value = directive_value.substr( 0,
						directive_value.find_first_of( "{};" ) );
				trim_f( directive_value, &std::isspace );
				if ( directive_value.empty() )
				{
					ERROR( "unnamed \"location\"." );
					return ( EXIT_FAILURE );
				}
				if ( directive_value.back() != '/' 
						&& directive_value.size() > 1
						&& directive_value[0] != '*' )
					directive_value.append( "/" );
				location.assign( directive_value );
				if ( this->_servers.back().setRoute( directive_value ) ) 
					return ( EXIT_FAILURE );
				directive_value.clear();
			}
			else
				context.pop();
			continue ;
		}
		else if ( directive == "}" )
		{
			location.clear();
			if ( context.top() == "server"
					&& this->_servers.back().check() == EXIT_FAILURE )
				return ( EXIT_FAILURE );
			context.pop();
			continue ;
		}
		directive_value = directive.substr( directive_name.length() );
		if ( directive_value.empty() == false && directive_value.back() == ';' )
			directive_value.erase( directive_value.length() - 1, 1 );
		trim_f( directive_value, &std::isspace );
		t_conf_opts * opt = get_option( directive_name, this->_opts );
		if ( opt->set_func( this->_servers.back(), directive_value, location ) )
			return ( EXIT_FAILURE );
		directive_value.clear();
	}
	if ( context.size() != 1 )
	{
		ERROR( "context \"" << context.top() << "\" unclosed" );
		return ( EXIT_FAILURE );
	}
	if ( compare_servers( this->_servers ) == EXIT_FAILURE )
		return ( EXIT_FAILURE );
	return ( EXIT_SUCCESS );
}

int
Router::listen ( void )
{
	int n_events = 1;
	struct kevent ev;
	IEvent * instance;

	for ( std::vector< Server >::iterator i = this->_servers.begin();
			i != this->_servers.end(); ++i )
	{
		//TODO: check if not active already
		if ( Router::isConnection( i->getListen() ) == EXIT_SUCCESS )
			continue ;
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
	while ( this->good() )
	{
		n_events = ::kevent( IEvent::kq, 0x0, 0, &ev, 1, 0 );
		if ( this->good() == false )
			return ( EXIT_FAILURE );
		if ( n_events == -1 )
		{
			ERROR( "kevent: " << std::strerror( errno ) );
			break ;
		}
		else if ( n_events == 0 )
		{
			continue ;
		}
		instance = static_cast< IEvent * >( ev.udata );
		instance->dispatch( ev );
	}
	return ( EXIT_SUCCESS );
}

int
Router::register_read_socket ( Connection & instance ) const
{
	struct kevent ev;

	EV_SET( &ev, instance.getSocketFD(), EVFILT_READ,
			EV_ADD | EV_ENABLE | EV_CLEAR, 0, 0, (void *) this );
	if ( ::kevent( IEvent::kq, &ev, 1, 0x0, 0, 0 ) == -1 )
	{
		ERROR( "kevent: " << std::strerror( errno ) );
		return ( EXIT_FAILURE );
	}
	return ( EXIT_SUCCESS );
}

void
Router::dispatch ( struct kevent & ev )
{
	HTTP * client;

	if ( ev.filter != EVFILT_READ )
	{
		WARN( "unknown event filter" );
		return ;
	}
	client = new HTTP( *this, ev.ident );
	// TODO: store clients somewhere to keep track of leaks
	(void) client;
	return ;
}

bool
Router::good ( void ) const
{
	return ( this->_good );
}

Connection &
Router::getConnection ( int fd )
{
	std::vector< Connection >::const_iterator it;

	it = this->_connections.begin();
	while ( it != this->_connections.end() )
	{
		if ( it->getSocketFD() == fd )
			return ( const_cast< Connection & >( *it ) );
		++it;
	}
	return ( this->_connections[0] );
}

Server &
Router::getServer ( std::string & server_name, in_addr_t host, in_port_t port )
{
	std::vector< Server >::iterator it;
	std::string mod_server_name( server_name );
	
	if ( mod_server_name.find( ':' ) != std::string::npos )
		mod_server_name.erase( mod_server_name.find( ':' ) );
	it = this->_servers.begin();
	while ( it != this->_servers.end() )
	{
		if ( it->hasServerName( mod_server_name ) == true
				&& port == it->getPort() && host == it->getHost() )
			return ( *it );
		it++;
	}
	return ( this->getDefaultServer() );
}

Server &
Router::getDefaultServer ( void )
{
	return ( this->_servers[0] );
}

int
Router::isConnection ( const struct sockaddr_in & address ) const
{
	std::vector< Connection >::const_iterator it;

	it = this->_connections.begin();
	while ( it != this->_connections.end() )
	{
		if ( std::memcmp( &(it->getAddress()), &address,
					sizeof (struct sockaddr_in) ) == 0 )
			return ( EXIT_SUCCESS );
		++it;
	}
	return ( EXIT_FAILURE );
}

int
Router::setConnection ( const struct sockaddr_in & address,
		int domain, int type, int protocol )
{
	this->_connections.push_back( Connection( address,
				domain, type, protocol ) );
	if ( this->_connections.back().good() == false )
		return ( EXIT_FAILURE );
	INFO( "listening to "
			<< std::hex << this->_connections.back().getHost()
			<< std::dec << ":" << this->_connections.back().getPort() );
	return ( EXIT_SUCCESS );
}

/* The purpose of the following setters,
 * set_*( Server &, std::string &, std::string = "" ),
 * is to be a wrapper to the Server's class setter function
 * of each directive defined in a t_conf_opts struct.
 *
 * These only convert the format, if needed, to be accepted 
 * by the apropiate setter.
 *
 * set_limit_except()
 * set_autoindex()
 * set_cgi_pass()
 * set_cgi_param()
 * set_client_max_body_size()
 * set_error_page()
 * set_index()
 * set_listen()
 * set_root()
 * set_server_name()
 * set_redirection()
 *
 */

int
set_limit_except( Server & instance, std::string & arg, std::string location )
{
	//TODO: implement t_methods from HTTP
	//TODO: return EXIT_FAILURE if setFlag returns EXIT_FAILURE
	std::istringstream iss( arg );
	std::string word;
	std::size_t limit_flag;
	
	if ( arg.empty() )
	{
		ERROR( "invalid number of arguments in \"limit_except\"" );
		return ( EXIT_FAILURE );
	}
	limit_flag = 0x11111111;
	limit_flag ^= F_AUTOINDEX;
	while ( iss >> word )
	{
		if ( word == "GET" )
		{
			limit_flag ^= HTTP_GET;
			limit_flag ^= HTTP_HEAD;
		}
		else if ( word == "PUT" )
			limit_flag ^= HTTP_PUT;
		else if ( word == "POST" )
			limit_flag ^= HTTP_POST;
		else if ( word == "HEAD" )
			limit_flag ^= HTTP_HEAD;
		else
		{
			ERROR( "invalid method \"" << word << "\"" );
			return ( EXIT_FAILURE );
		}
	}
	instance.setFlag( limit_flag, false, location );
	return ( EXIT_SUCCESS );
}

int
set_autoindex( Server & instance, std::string & arg, std::string location )
{
	if ( arg.empty() )
		return ( EXIT_FAILURE );
	else if ( arg == "off" )
		return ( instance.setFlag( F_AUTOINDEX, false, location ) );
	else if ( arg == "on" )
		return ( instance.setFlag( F_AUTOINDEX, true, location ) );
	else
		ERROR( "not a valid value for \"autoindex\"" );
	return ( EXIT_FAILURE );
}

// int
// set_cgi_param ( Server & instance, std::string & arg, std::string location )
// {
// 	return ( instance.setCGIparam( arg, location ) );
// }

int
set_cgi_pass ( Server & instance, std::string & arg, std::string location )
{
	return ( instance.setCGIpass( arg, location ) );
}

int
set_client_max_body_size( Server & instance, std::string & arg, std::string location )
{
	return ( instance.setClientMaxBodySize( arg, location ) );
}

int
set_error_page ( Server & instance, std::string & arg, std::string )
{
	std::istringstream iss( arg );
	std::string::iterator it;
	std::string num, page;

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
{
	std::istringstream iss( arg );
	std::string word;

	while ( iss >> word )
	{
		if ( instance.setIndex( word, location ) == EXIT_FAILURE )
			return ( EXIT_FAILURE );
	}
	return ( EXIT_SUCCESS );
};

int
set_listen( Server & instance, std::string & arg, std::string )
{
	struct sockaddr_in * address = NULL;
	struct addrinfo hints, * result, * rp;
	std::istringstream iss( arg );
	std::string ip, port;
	int ecode;
   
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
	while ( rp != NULL )
	{
		if ( rp->ai_family == AF_INET )
		{
			address = (struct sockaddr_in * ) rp->ai_addr;
			break ;
		}
		rp = rp->ai_next;
	}
	if ( address != NULL )
	{
		ecode = instance.setListen( *address );
		freeaddrinfo( result );
	}
	if ( address == NULL || ecode == EXIT_FAILURE )
	{
		LOG( "HERE" );
		return ( EXIT_FAILURE );
	}
	return ( EXIT_SUCCESS );
}

int
set_root ( Server & instance, std::string & arg, std::string location )
{
	if ( arg.empty() || arg.find( " " ) != std::string::npos )
	{
		ERROR( "invalid number of arguments in \"root\"" );
		return ( EXIT_FAILURE );
	}
	if ( arg[0] != '/' )
	{
		ERROR( "root must be an absolute path" );
		return ( EXIT_FAILURE );
	}
	return ( instance.setRoot( arg, location ) );
}

int
set_server_name ( Server & instance, std::string & arg, std::string )
{
	std::istringstream iss( arg );
	std::string word;
   
	if ( arg.empty() )
	{
		ERROR( "invalid number of arguments in \"server_name\"" );
		return ( EXIT_FAILURE );
	}
	while ( iss >> word )
	{
		if ( instance.setServerName( word ) == EXIT_FAILURE )
			return ( EXIT_FAILURE );
	}
	return ( EXIT_SUCCESS );
}

int
set_redirection ( Server & instance, std::string & arg, std::string location )
{
	return ( instance.setRedirection( arg, location ) );
}

int
set_upload_directory ( Server & instance, std::string & arg, std::string location )
{
	return ( instance.setUploadDirectory( arg, location ) );
}
