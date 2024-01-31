#include "webserv.hpp"

int
main (void)
{
	// set default values
	// localhost:80
	
	// create socket
	int sockfd = ::socket(AF_INET, SOCK_STREAM, 0x0);
	if ( sockfd == -1 )
	{
		std::cerr << "::socket: " << ::strerror(errno) << std::endl;
		return (0x1);
	}

	std::clog << "::socket: " << sockfd << std::endl;

	// address to bind socket to
	struct sockaddr_in addr = {};
	
	addr.sin_family = AF_INET;
	addr.sin_port = htons(80);
	addr.sin_addr.s_addr = htonl(INADDR_ANY);

	// bind socket to an address
	if ( ::bind(sockfd, (struct sockaddr*) &addr, sizeof(addr)) != 0)
	{
		std::cerr << "::bind " << ::strerror(errno) << std::endl;
		return (0x2);
	}

	std::clog << "::bind OK" << std::endl;

	// listen to socket
	if ( ::listen(sockfd, 0x0) != 0 )
	{
		std::cerr << "::listen " << std::endl;
		return (0x3);
	}

	std::clog << "::listen OK" << std::endl;

	// client's (request) sockaddr_in
	struct sockaddr_in client;
	socklen_t client_addrlen;

	// configure accept CALL
	while ( true )
	{
		std::clog << "waiting ::accept CALL" << std::endl;

		int newsockfd = ::accept(sockfd, 
				(struct sockaddr*) &client, 
				&client_addrlen);

		std::clog << "newsockfd: " << newsockfd << std::endl;

		if ( newsockfd < 0 )
		{
			std::cerr << "::accept " << ::strerror(errno) << std::endl;
			return (0x4);
		}

		std::clog << "closing " << newsockfd << std::endl;
		close(newsockfd);

	}

	// close socket
	::close(sockfd);

	return (0x0);
}
