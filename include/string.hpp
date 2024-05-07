/*                                                                            */
/* string.hpp                                                                 */
/* mpuig-ma <mpuig-ma@student.42barcelona.com>                                */
/* Wed Apr 10 14:46:01 2024                                                   */

#pragma once

#include <string>
#include "log.hpp"

std::string & normalize ( std::string & );
std::string & trim_comments ( std::string &, const char * );
std::string & trim_f( std::string &, int ( *func )( int ) );
std::string & replace_f ( std::string &, int ( *func )( int ), char );
std::string & del_multipl_sp( std::string &, int ( *func )( int ) );

std::string & urlencode ( std::string & url );
std::string & urldecode ( std::string & url );

std::string & strtolower ( std::string & );

std::size_t how_many_words ( const std::string & );
std::size_t how_many_characters_of ( const std::string & str, char c );
std::string get_word ( const std::string &, std::string delimiter );

bool compare_file_extension( const std::string & a, const std::string & b );
