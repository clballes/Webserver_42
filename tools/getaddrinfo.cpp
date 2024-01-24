/*                                                                            */
/* test_getaddrinfo.cpp                                                       */
/* mpuig-ma <mpuig-ma@student.42barcelona.com>                                */
/* Wed Jan 24 14:06:34 2024                                                   */

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <iostream>
#include <iomanip>
#include <arpa/inet.h>

int getaddrinformation (const char*, const char*);
void print_addrinfo (const struct addrinfo&, int);

void
print_addrinfo (const struct addrinfo& instance, int n)
{
	std::string value;

	++n;
	std::cout << std::left << "\033[4m";

	switch ( instance.ai_family )
	{
		case ( AF_INET ):
			value.assign("AF_INET");
			break ;
		case ( AF_INET6 ):
			value.assign("AF_INET6");
			break ;
		default:
			value.assign("other");
			break ;
	}
	
	std::cout << "(" << n << ") "
		<< std::setw(16) << "Family:"
		<< std::setw(16) << value
		<< std::setw(6) << instance.ai_family
		<< std::endl;

	switch ( instance.ai_socktype )
	{
		case ( SOCK_STREAM ):
			value.assign("SOCK_STREAM");
			break ;
		case ( SOCK_DGRAM ):
			value.assign("SOCK_DGRAM");
			break ;
		case ( SOCK_RAW ):
			value.assign("SOCK_RAW");
			break ;
		case ( SOCK_RDM ):
			value.assign("SOCK_RDM");
			break ;
		case ( SOCK_SEQPACKET ):
			value.assign("SOCK_SEQPACKET");
			break ;
		default:
			value.assign("other");
			break ;
	}

	std::cout << "(" << n << ") "
		<< std::setw(16) << "Socket type:"
		<< std::setw(16) << value
		<< std::setw(6) << instance.ai_socktype
		<< std::endl;

	switch ( instance.ai_protocol )
	{
		case ( IPPROTO_UDP ):
			value.assign("UDP");
			break ;
		case ( IPPROTO_TCP ):
			value.assign("TCP");
			break ;
		default:
			value.assign("other");
			break ;
	}

	std::cout << "(" << n << ") "
		<< std::setw(16) << "Protocol:"
		<< std::setw(16) << value
		<< std::setw(6) << instance.ai_protocol
		<< std::endl;

	std::cout << "\033[0m";

	return ;
}

int
getaddrinformation (const char* hostname, const char* servname)
{
	int error_code, i;
	struct addrinfo* res = 0x0;
	struct addrinfo* cpy;

	struct addrinfo hints;
	memset(&hints, 0x0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_ADDRCONFIG | AI_PASSIVE;

	std::cout << "Trying getaddrinfo with "
		<< hostname << " (" << servname << ")" << std::endl;
	
	error_code = getaddrinfo(hostname, servname, &hints, &res);
	if (error_code != 0)
	{
		std::cerr << gai_strerror(error_code) << std::endl;
		return (0x1);
	}
	
	if (res != 0x0)
	{
		i = 0;
		cpy = res;
		while (cpy != 0x0)
		{
			print_addrinfo(*cpy, i);
			++i;
			cpy = cpy->ai_next;
		}

		freeaddrinfo(res);
	}

	return (error_code);
}

int
main (int argc, const char** argv)
{

	char* hostname = 0x0;
	char* servname = 0x0;

	if (argc != 3)
	{
		std::cerr << "usage: " << argv[0] << " hostname servname" << std::endl;
		return (0x1);
	}

	if (argc >= 2)
		hostname = (char*) argv[1];

	if (argc == 3)
		servname = (char*) argv[2];

	if ( getaddrinformation(argv[1], argv[2]) == -1 )
		return (0x1);

	/*
	if ( getaddrinformation(std::string("google.com").c_str(),
				std::string("http").c_str()) == -1 ) { return (0x1); }

	if ( getaddrinformation(std::string("localhost").c_str(),
				std::string("http").c_str()) == -1 ) { return (0x1); }
	
	if ( getaddrinformation(std::string("10.11.8.5").c_str(),
				std::string("http").c_str()) == -1 ) { return (0x1); }

	if ( getaddrinformation(std::string("101.11.8.5").c_str(),
				std::string("http").c_str()) == -1 ) { return (0x1); }

	if ( getaddrinformation(std::string("101.11.8.2").c_str(),
				std::string("http").c_str()) == -1 ) { return (0x1); }
	*/

	return (0x0);
}
