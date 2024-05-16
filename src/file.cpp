/*                                                                            */
/* file.cpp                                                                   */
/* mpuig-ma <mpuig-ma@student.42barcelona.com>                                */
/* Wed Apr 10 14:49:21 2024                                                   */

#include "file.hpp"

bool
is_regular_file( const std::string & filename )
{
	struct stat file_info;

    if ( stat( filename.c_str(), &file_info ) == -1 )
        return ( false );
    return ( S_ISREG( file_info.st_mode ) );
}

bool
can_access_file ( const std::string & filename, int mask )
{
	if ( is_regular_file( filename ) == true
			&& access( filename.c_str(), mask ) == 0 )
            return ( true );
    return ( false ); 
}

bool routeExists( const std::string & route )
{
    return ( access( route.c_str(), 0 ) == 0 );
}

int
load_file ( const std::string & target, std::string & dst )
{
	std::ifstream file;
	std::ifstream::pos_type pos;

	file.open( target, std::ios::in | std::ios::binary | std::ios::ate );
	if ( file.good() == true && file.eof() == false )
	{
		// TODO: sanity checks
		pos = file.tellg();
		file.seekg( 0, std::ios::beg );
		dst.resize( pos );
		file.read( (char *) dst.data(), pos );
	}
	if ( file.good() == false )
	{
		WARN( target << ": " << std::strerror( errno ) );
		return ( FORBIDDEN );
	}
	return ( OK );
}

static std::vector< std::pair< std::string, std::string > >
parse_string( const std::string & input )
{
    std::vector<std::pair<std::string, std::string> > result;
    std::istringstream iss(input);
    std::string token;
    while (std::getline(iss, token, '&')) {
        size_t pos = token.find('=');
        if (pos != std::string::npos) {
            std::string key = token.substr(0, pos);
            std::string value = token.substr(pos + 1);
            result.push_back(std::make_pair(key, value));
        }
    }
    return ( result );
}

int
generate_html ( const t_request & request, std::string & dst )
{
    std::string	html, res;
	std::vector< std::pair< std::string, std::string > > pairs;
	std::vector< std::pair< std::string, std::string > >::const_iterator it;

    html.append( "<!DOCTYPE html>\n"
		"<html>\n"
		"<head>\n"
		"<title>POST Request Successful</title>\n"
		"<style>\n"
		// Adding some basic styles
		"body { font-family: Arial, sans-serif; }\n"
		"h1 { color: #333; }\n"
		"p { color: #666; }\n"
		"</style>\n"
		"</head>\n"
		"<body>\n"
		"<h1>POST Request Successful</h1>\n"
		"<p>Your POST request was successfully received and processed.</p>\n" );
	res.assign( request.body );
	(void) urldecode( res );
    pairs = parse_string( res );
    for ( it = pairs.begin(); it != pairs.end(); ++it )
	{
        html.append( "<p><strong>Key:</strong> " );
		html.append( it->first );
		html.append( " <strong>Value:</strong> " );
		html.append( it->second );
		html.append( "</p>\n" );
    }
    html.append( "</body>\n" );
    html.append( "</html>\n" );
    dst.append( html );
    return ( EXIT_SUCCESS );
}

/*
 * From RFC9112:
 * When making a request directly to an origin server,
 * other than a CONNECT or server-wide OPTIONS request
 * (as detailed below), a client MUST send only the
 * absolute path and query components of the target URI
 * as the request-target. If the target URI's path
 * component is empty, the client MUST send "/" as the
 * path within the origin-form of request-target.
 */

int
autoindex ( const t_request & request, std::string & dst )
{
	std::string page;
	std::string directory_name;
	DIR*        directory;

	directory_name = request.file;
	directory = opendir( directory_name.c_str() );
	if ( directory == NULL )
	{
		WARN( directory_name << ": " << strerror( errno ) );
		return ( NOT_FOUND );
	}
	// HTML init content tags ( html, head, title, body ).
	page.append( "<!DOCTYPE html>" );
	page.append( "<head>" );
	page.append( "<title>" );
	page.append( request.target );
	page.append( "</title>" );
	// Add style tags
	page.append( "<style>" );
	page.append( "img.icon {" );
	page.append( "    vertical-align: middle;" );
	page.append( "    margin-right: 8px;" );
	page.append( "}" );
	page.append( "a.link {" );
	page.append( "    vertical-align: middle;" );
	page.append( "}" );
	page.append( "</style>" );
	page.append( "</head>" );
	page.append( "<body>" );
	page.append( "<h1>Index of " );
	page.append( request.target );
	page.append( "</h1>" );
	page.append( "<table>" );
	page.append("<thead>" );
	page.append( "<tr class=\"header\" ><th tabindex=\"0\" role=\"button\">Name</th></tr></thead>" );
	page.append( "<tbody>" );
	for ( struct dirent *ent = readdir( directory );
			ent != 0x0; ent = readdir( directory ) )
	{
		if ( std::strcmp( ent->d_name, "." ) != 0
				&& std::strcmp( ent->d_name, ".." ) != 0 )
		{
			page.append( "<tr>" );
			page.append( "<td data-value=\"" );
			page.append( ent->d_name );
			page.append( "\">" );
			if ( ! is_regular_file( ent->d_name ) )
			{
				page.append( "<img src=\"/assets/dir_icon.png\" "
						"alt=\"Directory Icon\" "
						"style=\"width: 16px; "
						"height: 16px; "
						"padding-inline-start: 1em; "
						"vertical-align:middle; "
						"padding-inline-end: 0.7em;\">" );
			}
			else
			{
				page.append( "<img src=\"/assets/file_icon.png\" "
						"alt=\"File Icon\" "
						"style=\"width: 16px; "
						"height: 16px; "
						"padding-inline-start: 1em; "
						"vertical-align:middle; "
						"padding-inline-end: 0.7em;\">" );
			}
			page.append( "<a href=\"http://" );
			page.append( request.headers.at( "host" ) );
			if ( request.target.size() > 1 )
				page.append( request.target );
			if ( page.back() != '/' )
				page.append( "/" );
			page.append( ent->d_name );
			page.append( "\" style=\"vertical-align: middle;\">" );
			page.append( ent->d_name );
			page.append( "</a>" );
			page.append( "</td>" );
			page.append( "</tr>" );
		}
	}
	// HTML end tags
	page.append( "</body>" );
	page.append( "</html>" );
    closedir( directory );
	dst.append( page.c_str() );
	return ( OK );
}
