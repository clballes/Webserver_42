/*                                                                            */
/* setters.cpp                                                                */
/* mpuig-ma <mpuig-ma@student.42barcelona.com>                                */
/* Tue Feb 20 11:07:07 2024                                                   */

#include "ServerConf.hpp"
#include <sstream>

// renamed from check_directives

int
ServerConf::set_directives ( const std::deque< std::string > & server_block )
{
	std::deque< std::string >::const_iterator it = server_block.begin();
	std::string directive;
	std::string::size_type pos;

	LOG( "call check_directives()" )

	// There won't be "{" or "}" lines.
	// See fill_block().

	while ( it != server_block.end() )
	{
		pos = it->find( ' ' );

		// If pos == std::string::npos
		// means the string does not
		// container any `space';
		// the full string is the directive then.

		if ( pos == std::string::npos )
			pos = it->length();

		directive = it->substr( 0 , pos );

		LOG( "directive is: '" << directive << "'" )

		// The following may go inside another routine.:
		// Iterate over known configuration_directives.
		// If non is found, a NULL pointer will be returned.

		t_configuration_directives * ptr = &ServerConf::_config_directives[0];
		while ( ptr != NULL )
		{
			if ( directive.compare( ptr->directive_name ) == 0 )
				break ;
			++ptr;
		}

		if ( ptr == NULL )
		{
			std::cerr << PROGRAM_NAME;
			std::cerr << ": error: invalid directive";
			std::cerr << std::endl;
			return ( EXIT_FAILURE );
		}

		// Execute function to set whatever the directive found is.

		ptr->set_func( *this, it->c_str() );

		++it;
	}

	return ( EXIT_SUCCESS );
}

bool is_valid_ipv4( const std::string & ip )
{
    std::istringstream iss( ip);
    int num;
    char dot;

    for ( int i = 0; i < 4; ++i )
    {
        if ( ! ( iss >> num )
            || ( i < 3 && ! ( iss >> dot ) && dot != '.')
			|| ( num < 0 || num > 255 ) )
            return ( false );
    }
    return ( true );
}

int
port_digit ( std::string portStr )
{
	std::string::size_type i;

    for ( i = 0; i < portStr.length(); i++ )
	{
        if ( ! isdigit( portStr[i] ) )
        {
            std::cerr << "Error number port" << std::endl;
            return ( 0 );
        }
    }

    return ( 1 );
}

int
ServerConf::set_listen ( ServerConf & conf, const char * arg )
{
	std::string str( arg );
    std::istringstream iss( arg );
    std::string ip, portStr;

	std::string::size_type pos = str.find( ':' );

	if ( pos != std::string::npos )
    {
        std::getline( iss, ip, ':' ); // tneim la ip
        std::getline( iss, portStr ); //tenim el port
    }
    else
    {
        if ( str.length() >= 7 ) // vol dir q trobem una ip
        {
            ip = str;
            conf._port = 80;
        }
        else
        {
            portStr = str; // vol dir q tenim el port
			conf._host = INADDR_ANY;
        }
    }
    if ( ip == "localhost" )
    {
        conf._host = INADDR_LOOPBACK;
        if ( pos == std::string::npos )
            conf._port = 80;
    }
    else if ( ( ! ip.empty() && !is_valid_ipv4( ip ) ) )
	{
        std::cerr << "IP address not ipv4" << std::endl;
        //_status = false;
    }
    else
        conf._host = inet_addr( ip.c_str() );

    if ( port_digit( portStr ) == 0 && conf._port != 80 )
	{
        std::cerr << "Error: Error syntax port, just numbers" << std::endl;
        //_status = false;
        return ( EXIT_FAILURE );
    }
    else if ( conf._port != 80 )
	{
        conf._port = std::stoi( portStr );
    }

	return ( EXIT_SUCCESS );
}

int
ServerConf::set_root ( ServerConf & conf, const char * arg )
{
	(void) conf;
	(void) arg;

	conf._root = arg;

	return ( EXIT_SUCCESS );
}

// alphanumeric characters, minus signs ("-"), and periods (".").
// They must begin with an alphabetic character and end with an
// alphanumeric character.

int
parse_server_name ( const std::string & str )				                   // @clballes
																			   // Aquesta funció només
																			   // es fa servir una vegada.
																			   // Potser la podem incorporar
																			   // directament a set_server_name.
{
	std::string::size_type i;

    for ( i = 0; i < str.length(); i++ )
    {
        if ( str[0] == '\"' )
            return ( 1 );
        if ( ! isalpha( str[0] ) && i == 0 )
            return ( 0 );
        else if ( ! isalnum( str[i] ) && ( str[i] != '-' && str[i] != '.' ) )
            return ( 0 );
    }

    return ( 1 );
}

int
ServerConf::set_server_name ( ServerConf & conf, const char * arg )
{
    std::istringstream iss( arg );
    std::vector< std::string > words;
    std::string word;

    while ( iss >> word )
	{
        if ( parse_server_name( word ) != 1 )
        {
            std::clog << " -------------- - -- KO SERVERNAME";
			std::clog << std::endl;
            // _status = false; mirar el default 0.0.0.0.					   // @clballes
            return ( EXIT_FAILURE );
        }
        else
            conf._server_name.push_back( word );
    }

	return ( EXIT_SUCCESS );
}

int
ServerConf::set_error_page ( ServerConf & conf, const char * arg )
{
	(void) conf;
	(void) arg;

	LOG( "error_page is: " << arg )

	return ( EXIT_SUCCESS );
}

int
ServerConf::set_client_body ( ServerConf & conf, const char * arg )
{
	(void) conf;
	(void) arg;

    int alpha = 10;
	std::string str( arg );

    conf._client_max_body_size = 0;

    if ( str == "\"\"" )
        return ( 1 );

    for( std::size_t i = 0; i < str.length(); i++ )
    {
        if ( ! isdigit( str[i] )
				&& str[i] != 'm' && str[i] != 'M'
				&& str[i] != 'k' && str[i] != 'K' )
            //_status = false;
        
		if ( isalpha( str[i] ) )
            alpha = 1;
    }
    
	if ( alpha == 1 )
    {
        std::string sub = str.substr( 0, str.length() - 1 );
        str = sub;
    }

    int intValue = std::atoi( str.c_str() );
    conf._client_max_body_size = static_cast<std::size_t>( intValue );

	return ( EXIT_SUCCESS );
}

int
ServerConf::set_cgi_param ( ServerConf & conf, const char * arg )
{
	(void) conf;
	(void) arg;
	
	conf._cgi_param = arg;

	return ( EXIT_SUCCESS );
}

int
ServerConf::set_cgi_pass ( ServerConf & conf, const char * arg )
{
	(void) conf;
	(void) arg;

	conf._cgi_pass = arg;

	return ( EXIT_SUCCESS );
}

int
ServerConf::set_allow_methods ( ServerConf & conf, const char * arg )
{
	(void) conf;
	(void) arg;

	std::istringstream iss( arg );
	std::string word;

	while ( iss >> word )
	{
		if ( word != "GET" && word != "POST" )
		{
			//_status = false;
			return ( EXIT_FAILURE );
		}
		else
			conf._allow_methods.push_back( word );
	}

	return ( EXIT_SUCCESS );
}

int
ServerConf::set_index ( ServerConf & conf, const char * arg )
{
	(void) conf;
	(void) arg;

	conf._index = arg;

	return ( EXIT_SUCCESS );
}

int
ServerConf::set_autoindex ( ServerConf & conf, const char * arg )
{
	(void) conf;
	(void) arg;

	conf._autoindex = arg;

	return ( EXIT_SUCCESS );
}
