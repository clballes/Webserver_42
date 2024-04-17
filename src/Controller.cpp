/*                                                                            */
/* Controller.cpp                                                             */
/* mpuig-ma <mpuig-ma@student.42barcelona.com>                                */
/* Wed Apr  3 12:50:43 2024                                                   */

#include "Controller.hpp"

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
Controller::load ( std::string filename )
{
	std::string   line;
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
			this->_buffer.append( line ).append( " " );
	}
	if ( file.good() == false && file.eof() != true )
	{
		ERROR( filename << ": " << strerror( errno ) );
		return ( EXIT_FAILURE );
	}
	trim_f( this->_buffer, &std::isspace );
	file.close();
	LOG( this->_buffer.c_str() );
	if ( parse() == EXIT_FAILURE )
	{
		this->_good = false;
		return ( EXIT_FAILURE );
	}
	return ( EXIT_SUCCESS );
}

int
Controller::parse( void )
{
	std::stack< std::string >	context;
	std::string					directive;
	std::size_t					position, directive_len;

	position = 0;
	while ( position < this->_buffer.length() )
	{
		directive_len = get_directive_length( this->_buffer, position );
		directive = this->_buffer.substr( position, directive_len );
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
		if ( get_word( directive ) == "server" )
		{

		}
		else if ( get_word( directive ) == "location" )
		{}
	}
	return ( EXIT_SUCCESS );
}

/*
int
Controller::setup_instances ( void )
{
	//TODO: ERROR when directive not terminated with ;
	std::size_t pos, directive_len, i;
	std::string	directive, word, argument;
	std::stack< std::string > context;
	Server * current_instance = nullptr;

	(void) current_instance;
	pos = 0;
	context.push( "main" );
	while ( pos < this->_buffer.length() )
	{
		directive_len = get_directive_length( this->_buffer, pos );
		directive = this->_buffer.substr( pos, directive_len );
		trim_f( directive, &std::isspace );
		word = directive.substr( 0, directive.find_first_of( " \f\n\r\t\v{};" ) );
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
				//this->_instances.resize( this->_instances.size() + 1 );
				//current_instance = &this->_instances.back();
			}
			else if ( word == "location" )
			{
				argument = directive.substr( word.length(),
						directive.find_first_of( " \f\n\r\t\v{};" ) + word.length() );
				trim_f( argument, &std::ispunct );
				trim_f( argument, &std::isspace );

				// TODO
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
			//if ( Controller::_opts[i].set_func( *current_instance, arg ) )
			//	return ( EXIT_FAILURE );

		}
		else if ( context.top() == "location" )
		{
			i = 0;
			while ( i < how_many_options_are_there( Controller::_opts )
					&& word.compare( Controller::_opts[i].identifier ) != 0 )
				++i;
			if ( i == how_many_options_are_there( Controller::_opts ) )
				return ( EXIT_FAILURE );
			// TODO
		}
		else
		{
			// TODO
			std::cout << "else\n";
		}
	}
	return ( EXIT_SUCCESS );
}
*/

int
set_allow_methods( Server & instance, std::string & arg )
{
	//TODO: implement t_methods from HTTP
	//TODO: return EXIT_FAILURE if setFlag returns EXIT_FAILURE
	std::istringstream iss( arg );
	std::string word;

	//DEBUG( arg.c_str() );
	if ( arg.empty() )
	{
		ERROR( "invalid number of arguments in \"allow_methods\"" );
		return ( EXIT_FAILURE );
	}
	while ( iss >> word )
	{
		if ( word == "GET" )
			instance.setFlag( METHOD_GET, true );
		else if ( word == "PUT" )
			instance.setFlag( METHOD_PUT, true );
		else if ( word == "POST" )
			instance.setFlag( METHOD_POST, true );
		else if ( word == "HEAD" )
			instance.setFlag( METHOD_HEAD, true );
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
	//DEBUG( arg.c_str() );
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
set_cgi_param ( Server & instance, std::string & arg )
{
	//TODO: what if a path has spaces ???
	//DEBUG( arg.c_str() );
	if ( arg.empty() || arg.find( " " ) != std::string::npos )
	{
		ERROR( "invalid number of arguments in \"cgi_pass\"" );
		return ( EXIT_FAILURE );
	}
	return ( instance.setCGIparam( arg ) );
}

int
set_cgi_pass ( Server & instance, std::string & arg )
{
	//TODO: what if a path has spaces ???
	//DEBUG( arg.c_str() );
	if ( arg.empty() || arg.find( " " ) != std::string::npos )
	{
		ERROR( "invalid number of arguments in \"cgi_pass\"" );
		return ( EXIT_FAILURE );
	}
	return ( instance.setCGIpass( arg ) );
}

int
set_client_body ( Server & instance, std::string & arg )
{
	//TODO: multiply value for 'M, m, K, k'
	std::size_t n = 0;
	int alpha = 0;

	//DEBUG( arg.c_str() );
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
set_error_page ( Server & instance, std::string & arg )
{
	//TODO: expects 2 arguments only
	std::istringstream iss( arg );
	std::string::iterator it;
	std::string num, page;

	//DEBUG( arg.c_str() );
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
set_index ( Server & instance, std::string & arg )
{
	std::istringstream iss( arg );
	std::string word;

	//DEBUG( arg.c_str() );
	if ( arg.empty() )
	{
		ERROR( "invalid number of arguments in \"index\"" );
		return ( EXIT_FAILURE );
	}
	while ( iss >> word )
	{
		if ( instance.setIndex( word ) == EXIT_FAILURE )
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

	//DEBUG( arg.c_str() );
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
	//DEBUG( arg.c_str() );
	if ( arg.empty() || arg.find( " " ) != std::string::npos )
	{
		ERROR( "invalid number of arguments in \"root\"" );
		return ( EXIT_FAILURE );
	}
	return ( instance.setRoot( arg ) );
}

int
set_server_name ( Server & instance, std::string & arg )
{
	std::istringstream iss( arg );
	std::string word;

	//DEBUG( arg.c_str() );
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
