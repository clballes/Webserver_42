/*                                                                            */
/* setters.cpp                                                                */
/* mpuig-ma <mpuig-ma@student.42barcelona.com>                                */
/* Tue Feb 20 11:07:07 2024                                                   */

/* vim:set colorcolumn=80 :                                                   */

#include "ServerConf.hpp"
#include <sstream>

int
ServerConf::set_directives ( const std::deque< std::string > & server_block )
// Renamed from check_directives()
{
	std::deque< std::string >::const_iterator it = server_block.begin();

	LOG( "call set_directives()" )

	// There won't be "{" or "}" lines.
	// See fill_block().
	while ( it != server_block.end() )
	{
		size_t posSpace = it->find(' ');
		size_t posLen = it->length();
		size_t pos = (posSpace != std::string::npos) ? posSpace : posLen;
		std::string directive = (pos != std::string::npos) ? it->substr(0, pos) : *it;
		std::string str = (pos != std::string::npos) ? it->substr(pos + 1) : *it;
		// The following may go inside another routine.:
		// Iterate over known configuration_directives.
		// If non is found, a NULL pointer will be returned.

		t_configuration_directives * ptr = &ServerConf::_config_directives[0];
		while ( ptr->directive_name != NULL )
		{	
			if ( directive.compare( ptr->directive_name ) == 0 )
				break ;
			++ptr;
		}

		if ( ptr->directive_name == NULL )
		{
			std::cerr << PROGRAM_NAME;
			std::cerr << ": error: invalid directive";
			std::cerr << std::endl;
			return ( EXIT_FAILURE );
		}
		// Execute function to set whatever the directive found is.
		if (ptr->set_func( *this,  str.c_str() ) == EXIT_FAILURE) //|| ptr->set_func( *this, str.c_str() ) == '\0' NO MENRECORDO EN QUIN CAS SINO NO EM DONAVA 
			return (EXIT_FAILURE);
		++it;
	}
	return ( EXIT_SUCCESS );
}

//valid port numbers 1 to 65535.
int
ServerConf::set_listen ( ServerConf & conf, const char * arg )
{
    std::istringstream iss( arg );
    std::string ip, portStr;
	const char *port = arg;

	LOG( "call set_listen()" )

    std::string::size_type pos = std::string(arg).find(':');
	if ( pos != std::string::npos )
	{
		std::getline( iss, ip, ':' ); // tenim la ip
		std::getline( iss, portStr ); //tenim el port
	}
	else if (strlen(arg) > 6 )
	{
		port = "80";
		std::getline( iss, ip );
	}

	struct addrinfo hints, *result, *rp;

    std::memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_INET;    // Allow IPv4 or IPv6 AF_UNSPEC
    hints.ai_socktype = SOCK_STREAM; // TCP socket
	hints.ai_flags = AI_PASSIVE;      // Listen on all available interfaces if not ip given
	int status = getaddrinfo(ip.empty() ? nullptr : ip.c_str(), portStr.empty() ? port : portStr.c_str(), &hints, &result);
	if (status != 0) {
		std::cerr << "getaddrinfo error: " << gai_strerror(status) << std::endl;
		return (EXIT_FAILURE);
	}
    for (rp = result; rp != nullptr; rp = rp->ai_next) {
        if (rp->ai_family == AF_INET) {
            struct sockaddr_in *ipv4 = (struct sockaddr_in *)rp->ai_addr;
            // Set the sockaddr_in structure
            conf._address.sin_family = ipv4->sin_family;
            conf._address.sin_port = ipv4->sin_port;
            conf._address.sin_addr = ipv4->sin_addr;
			break ;
        }
    }
	freeaddrinfo(result); // Free the dynamically allocated memory
	return (EXIT_SUCCESS);
}

int
ServerConf::set_root ( ServerConf & conf, const char * arg )
{
	(void) conf;
	(void) arg;

	conf._root = arg;

	return ( EXIT_SUCCESS );
}

// Server_name accoridng to nginx: alphanumeric characters, minus signs ("-"), and periods (".").
// They must begin with an alphabetic character and end with an alphanumeric character.

int
ServerConf::set_server_name ( ServerConf & conf, const char * arg )
{
    std::istringstream iss( arg );
    std::vector< std::string > words;
    std::string word;

	LOG( "call set_server_name()" )

    while ( iss >> word )
	{
		std::string::size_type i;
		if ( ! isalpha( word[0] ) ){
			return ( EXIT_FAILURE );
		}
		for ( i = 0; i < word.length(); i++ )
		{
			if ( ! isalnum( word[i] ) && ( word[i] != '-' && word[i] != '.' ) ){
            	return ( EXIT_FAILURE );
			}
		}
        conf._server_name.push_back( word );
    }
	if (word.empty()){
		// Empty server name ""

		conf._server_name.push_back( "\"\"" );
	}		
	return ( EXIT_SUCCESS );
}

int
ServerConf::set_error_page ( ServerConf & conf, const char * arg )
{
	LOG( "call set_error_page()" )

	(void) conf;
	(void) arg;

	LOG( "error_page is: " << arg )

	return ( EXIT_SUCCESS );
}

int
ServerConf::set_client_body ( ServerConf & conf, const char * arg )
{

	LOG( "call set_client_body()" )

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
	LOG( "call set_cgi_param()" )

	(void) conf;
	(void) arg;
	
	conf._cgi_param = arg;

	return ( EXIT_SUCCESS );
}

int
ServerConf::set_cgi_pass ( ServerConf & conf, const char * arg )
{
	LOG( "call set_cgi_pass()" )
	
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

	LOG( "call set_allow_methods()" )

	std::istringstream iss( arg );
	std::string word;

	while ( iss >> word )
	{
		if ( word != "GET" && word != "POST" )
		{
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

	LOG( "call set_index()" )

	conf._index = arg;

	return ( EXIT_SUCCESS );
}

int
ServerConf::set_autoindex ( ServerConf & conf, const char * arg ) //ferho
{
	(void) conf;
	(void) arg;

	LOG( "call set_autoindex()" )

	conf._autoindex = arg;

	return ( EXIT_SUCCESS );
}
