#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <iostream>
#include <arpa/inet.h>

int
main (int argc, const char** argv)
{
	int					error_code;
	struct addrinfo		hints = {};
	struct addrinfo*	res = 0x0;

	if (argc != 2)
		return (0x1);

	(void) hints;

	error_code = getaddrinfo(argv[1], "http", 0x0, &res);
	if (error_code != 0)
	{
		std::cerr << gai_strerror(error_code) << std::endl;
		return (0x2);
	}

	std::cout << "res: " << res << std::endl;

	struct addrinfo* it;
   	it = res;


	struct sockaddr_in* ptr;
	char str[INET6_ADDRSTRLEN];

	while (it != 0x0)
	{
		if (it->ai_addr->sa_family == AF_INET)
		{
			ptr = (struct sockaddr_in*) it->ai_addr;
			std::cout << inet_ntop(AF_INET, &ptr->sin_addr, str, INET6_ADDRSTRLEN) << std::endl;
		}
		it = it->ai_next;
	}

	uint32_t ip = ptr->sin_addr.s_addr;
	(void)ip;

	int s = socket(AF_INET, SOCK_STREAM, 6);
	std::cout << "s: " << s << std::endl;
	if (s == -1)
		std::cerr << strerror(errno) << std::endl;

	int b = bind(s, res[0].ai_addr, res[0].ai_addrlen);
	std::cout << "b: " << b << std::endl;
	if (b == -1)
		std::cerr << strerror(errno) << std::endl;

	int l = listen(s, 0);
	std::cout << "l: " << l << std::endl;
	if (l == -1)
		std::cerr << strerror(errno) << std::endl;

	return (0x0);
}
