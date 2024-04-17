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

std::string
get_word( std::string & str )
{
	std::string word;

	if ( str.find_first_of( " \f\n\r\t\v;{}" ) != std::string::npos )
		word.assign( str.substr( 0, str.find_first_of( " \f\n\r\t\v;{}" ) ) );
	return ( word );
}

std::size_t
get_directive_length( std::string & buffer, std::size_t position )
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
validate_directive( std::string & directive, const t_conf_opts * opts )
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
Router::listen ( void )
{
	int n_events = 1;
	struct kevent ev;
	IEvent * instance;

	DEBUG( IEvent::kq );
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
	LOG( buffer.c_str() );
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
	std::string					directive;
	std::size_t					position, directive_len;

	position = 0;
	while ( position < buffer.length() )
	{
		directive_len = get_directive_length( buffer, position );
		directive = buffer.substr( position, directive_len );
		position += directive_len;
		trim_f( directive, &std::isspace );
		if ( validate_directive( directive, this->_opts ) == EXIT_FAILURE )
			return ( EXIT_FAILURE );
		if ( context.empty() && directive == "}" )
		{
			ERROR( "error: context: }" );
			return ( EXIT_FAILURE );
		}
		else if ( directive.back() == '{' )
		{
			context.push( get_word( directive ) );
			continue ;
		}
		else if ( directive == "}" )
		{
			context.pop();
			continue ;
		}
		// hardcoded section

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

int
Router::setConnection ( struct sockaddr_in & address,
		int domain, int type, int protocol )
{
	this->_connections.push_back( Connection( address,
				domain, type, protocol ) );
	return ( EXIT_SUCCESS );
}

int
Router::setServer ( std::vector< std::string > & server_names )
{
	(void) server_names;
	return ( EXIT_SUCCESS );
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

int set_allow_methods ( void ) { return ( 0 ); };
int set_autoindex ( void ) { return ( 0 ); };
int set_cgi_param ( void ) { return ( 0 ); };
int set_cgi_pass ( void ) { return ( 0 ); };
int set_client_body ( void ) { return ( 0 ); };
int set_error_page ( void ) { return ( 0 ); };
int set_index ( void ) { return ( 0 ); };
int set_listen( void ) { return ( 0 ); };
int set_root ( void ) { return ( 0 ); };
int set_server_name ( void ) { return ( 0 ); };
