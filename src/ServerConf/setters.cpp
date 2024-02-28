/*                                                                            */
/* setters.cpp                                                                */
/* mpuig-ma <mpuig-ma@student.42barcelona.com>                                */
/* Tue Feb 20 11:07:07 2024                                                   */

/* vim:set colorcolumn=80 :                                                   */

#include "ServerConf.hpp"
#include <sstream>

int
ServerConf::set_directives ( const std::deque< std::string > & server_block )
{
	std::deque< std::string >::const_iterator it = server_block.begin();
	std::string directive;
	std::string::size_type pos;

	LOG( "call set_directives()" );

	// There won't be "{" or "}" lines.
	// See fill_block().

	while ( it != server_block.end() )
	{
		// previously transform isspace into only space
		pos = it->find( ' ' );

		if ( pos == std::string::npos )
			pos = it->length();

		directive = it->substr( 0, pos );
		LOG( " " << directive );

		if ( ServerConf::set_directive( directive,
				   directive.c_str() + pos ) == EXIT_FAILURE )
			return ( EXIT_FAILURE );

		++it;
	}
	
	return ( EXIT_SUCCESS );
}

int
ServerConf::set_directive ( std::string & directive, const char * arg )
{
	t_configuration_directives * ptr = &ServerConf::_config_directives[0];

	LOG( " call set_directive()" );
	LOG( " " << arg );

	if ( arg == NULL)
		std::cout << "ARGG\n";

	while ( ptr != NULL )
	{
		if ( directive.compare( ptr->directive_name ) == 0 )
			break ;
		++ptr;

	if ( ptr == NULL )
		std::cout << "reached end\n";
	}



	return ( EXIT_SUCCESS );
}

int
ServerConf::set_listen ( ServerConf & conf, const char * arg )
{(void) conf; (void) arg; return (0);}

int
ServerConf::set_root ( ServerConf & conf, const char * arg )
{(void) conf; (void) arg; return (0);}

int
ServerConf::set_server_name ( ServerConf & conf, const char * arg )
{(void) conf; (void) arg; return (0);}

int
ServerConf::set_error_page ( ServerConf & conf, const char * arg )
{(void) conf; (void) arg; return (0);}

int
ServerConf::set_client_body ( ServerConf & conf, const char * arg )
{(void) conf; (void) arg; return (0);}

int
ServerConf::set_cgi_param ( ServerConf & conf, const char * arg )
{(void) conf; (void) arg; return (0);}

int
ServerConf::set_cgi_pass ( ServerConf & conf, const char * arg )
{(void) conf; (void) arg; return (0);}

int
ServerConf::set_allow_methods ( ServerConf & conf, const char * arg )
{(void) conf; (void) arg; return (0);}

int
ServerConf::set_index ( ServerConf & conf, const char * arg )
{(void) conf; (void) arg; return (0);}

int
ServerConf::set_autoindex ( ServerConf & conf, const char * arg )
{(void) conf; (void) arg; return (0);}
