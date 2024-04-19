/*                                                                            */
/* Location.hpp                                                               */
/* mpuig-ma <mpuig-ma@student.42barcelona.com>                                */
/* Wed Apr 17 17:10:53 2024                                                   */

#ifndef _LOCATION_HPP_
#define _LOCATION_HPP_

#include <string>
#include <vector>
#include "log.hpp"

class Location
{
	public:

		Location ( void );
		~Location ( void );

		void setDefault ( void );
		int setFlag ( int, bool );
		int setCGIparam ( std::string & );
		int setCGIpass ( std::string & );
		int setRoot ( std::string & );
		int setIndex ( std::string & );	

		bool isDefault ( void ) const;
		bool getFlag ( int ) const;
		std::size_t getFlags ( void ) const;
		const std::string & getCGIparam ( void ) const ;
		const std::string & getCGIpass ( void ) const;
		const std::string & getRoot ( void ) const;
		std::vector< std::string > & getIndex ( void ) const;

	private:

		std::size_t					_flags;
		std::string					_cgi_param;
		std::string					_cgi_pass;
		std::string					_root;
		bool						_isDefault;
		std::vector< std::string >	_index;

};

#endif /* !_LOCATION_HPP_ */
