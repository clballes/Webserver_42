#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <iostream>
#include <arpa/inet.h>

int getaddrinformation (const char*, const char*);
void print_addrinfo (const struct addrinfo&);

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
	hints.ai_protocol = IPPROTO_IP;

	std::cout << "Trying getaddrinfo with "
		<< hostname << " (" << servname << ")" << "... ";
	
	error_code = getaddrinfo(hostname, servname, 0x0, &res);
	if (error_code != 0)
	{
		std::cerr << gai_strerror(error_code) << std::endl;
		return (0x1);
	}
	
	std::cout << error_code << std::endl;

	if (res != 0x0)
	{
		i = 0;
		cpy = res;
		while (cpy != 0x0)
		{
			++i;
			cpy = cpy->ai_next;
		}

		std::cout << "N. nodes: " << i << std::endl;		
		
		freeaddrinfo(res);
	}

	return (error_code);
}

int
main (void)
{
	if ( getaddrinformation(std::string("127.0.0.1").c_str(),
				std::string("http").c_str()) == -1 ) { return (0x1); }

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

	return (0x0);
}
