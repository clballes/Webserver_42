/*                                                                            */
/* Location.hpp                                                               */
/* mpuig-ma <mpuig-ma@student.42barcelona.com>                                */
/* Wed Apr 17 17:10:53 2024                                                   */

#ifndef _LOCATION_HPP_
#define _LOCATION_HPP_

#include <string>
#include <vector>
#include <cstdlib>		/* EXIT_SUCCESS, EXIT_FAILURE */
#include "define.hpp"
#include "log.hpp"

class Location
{
	public:

		Location ( void );
		~Location ( void );

		void setDefault ( void );
		int setFlag ( int, bool );
		int setCGIparam ( const std::string & );
		int setCGIpass ( const std::string & );
		int setRoot ( const std::string & );
		int setIndex ( const std::string & );
		int setUploadFiles ( const std::string & );
		int setRedirection ( const std::string & );

		bool isDefault ( void ) const;
		bool getFlag ( int ) const;
		std::size_t getFlags ( void ) const;
		const std::string & getCGIparam ( void ) const ;
		const std::string & getCGIpass ( void ) const;
		const std::string & getRoot ( void ) const;
		const std::vector< std::string > & getIndex ( void ) const;

		void log_conf ( void ) const;

	private:

		std::size_t					_flags;
		std::string					_cgi_param;
		std::string					_cgi_pass;
		std::string					_root;
		bool						_isDefault;
		std::vector< std::string >	_index;
		std::string					_upload_files;
		std::string					_redirection;

};

#endif /* !_LOCATION_HPP_ */
