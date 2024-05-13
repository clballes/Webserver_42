/*                                                                            */
/* file.cpp                                                                   */
/* mpuig-ma <mpuig-ma@student.42barcelona.com>                                */
/* Wed Apr 10 14:49:21 2024                                                   */

#include "file.hpp"
#include <unistd.h>
#include <iostream>
#include <sys/stat.h>
#include "HTTP.hpp" // Include the header file where HTTP is defined


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

template< typename T >
std::string my_to_string ( const T & value )
{
    std::stringstream ss;

    ss << value;
    return ( ss.str() );
}

template std::string my_to_string< int > ( const int & );
template std::string my_to_string< unsigned long > ( const unsigned long & );
template std::string my_to_string< unsigned short > ( const unsigned short & );

std::string tolower_string(const std::string& str)
{
    // Convert string to lowercase
    std::string result = str;
    for (size_t i = 0; i < result.length(); ++i) {
        result[i] = std::tolower(result[i]);
    }
    return result;
}

std::vector<std::pair<std::string, std::string> > parse_string(const std::string& input)
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
    return result;
}



int generate_html( HTTP & http )
{
    std::ostringstream html;
    html << "<!DOCTYPE html>\n";
    html << "<html>\n";
    html << "<head>\n";
    html << "<title>POST Request Successful</title>\n";
    html << "<style>\n";
    // Adding some basic styles
    html << "body { font-family: Arial, sans-serif; }\n";
    html << "h1 { color: #333; }\n";
    html << "p { color: #666; }\n";
    html << "</style>\n";
    html << "</head>\n";
    html << "<body>\n";
    html << "<h1>POST Request Successful</h1>\n";
    html << "<p>Your POST request was successfully received and processed.</p>\n";
    std::string res = urldecode(http.getRequest().body);
    std::vector<std::pair<std::string, std::string> > pairs = parse_string(res);
    for (std::vector<std::pair<std::string, std::string> >::const_iterator it = pairs.begin(); it != pairs.end(); ++it) {
        html << "<p><strong>Key:</strong> " << it->first << " <strong>Value:</strong> " << it->second << "</p>\n";
    }
    html << "</body>\n";
    html << "</html>\n";
    http.setMessageBody(html.str());
    return (EXIT_SUCCESS);
}