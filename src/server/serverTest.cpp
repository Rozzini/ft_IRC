#include <iostream>
#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <fcntl.h>
#include <poll.h>
#include <vector>

typedef std::vector<pollfd>::iterator       pfd_iterator;

void	client_Connect(int sock_fd, std::vector<pollfd>    _pfds)
{
	// accept a connection

	int			fd;
	sockaddr_in	addr = {};
	socklen_t	size = sizeof(addr);

	fd = accept(sock_fd, (sockaddr *) &addr, &size);
	if (fd < 0)
	{
		std::cerr << "Error while accepting a new client!" << std::endl;
		return ;
	}

	//	including the client fd in the poll

	pollfd  pfd = {fd, POLLIN, 0};
	_pfds.push_back(pfd);

	//	getting hostname from the client address

	char hostname[NI_MAXHOST];
	int res = getnameinfo((struct sockaddr *) &addr, sizeof(addr), hostname, NI_MAXHOST, NULL, 0, NI_NUMERICSERV);
	if (res != 0)
	{
		std::cerr << "Error while getting a hostname on a new client!" << std::endl;
		return ;
	}

    // creating and saving a new client

			//Client* client = new Client(fd, ntohs(addr.sin_port), hostname);
			//_clients.insert(std::make_pair(fd, client));

    // logging connect message

    char message[1000];
    sprintf(message, "%s:%d has connected.", "host", addr.sin_port);
    //log(message);
}





int main()
{
	int	_running = 1;
	
	std::vector<pollfd>     _pfds;
	//	Opening socket
	//	socket		-	create an endpoint for communication
	//	AF_INET		-	IPv4 Internet protocols
	//	SOCK_STREAM -	two-way, connection-based byte streams
	int sock_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (sock_fd == -1)
	{
		std::cerr << "Cant create a socket!" << std::endl;
		return -1;
	}

	//	forcefully attaching socket to the port by making it reusable
	//	setsockopt — set the socket options
    int optval = 1;
    if (setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)))
	{
		std::cerr << "Error while setting socket options!" << std::endl;
		return -1;
	}

	//	making the socket NON-BLOCKING
	//	fcntl - manipulate file descriptor
    if (fcntl(sock_fd, F_SETFL, O_NONBLOCK))
	{
		std::cerr << "Error while setting socket to NON-BLOCKING!" << std::endl;
		return -1;
	}

	// gathering neccessary data for binding

	struct sockaddr_in  serv_addr = {};

	bzero((char*) &serv_addr, sizeof(serv_addr));

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	//serv_addr.sin_port = htons(atoi(_port.c_str()));
	serv_addr.sin_port = htons(6667);

	//	bind socket to an IP address on selected port

	if (bind(sock_fd, (sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
	{
		std::cerr << "Error while binding a socket!" << std::endl;
		return -1;
	}

	//	listening for requests
	//	socket that will be used to accept incoming connection requests
	if (listen(sock_fd, 1000) < 0)
	{
		std::cerr << "Error while listening on a socket!" << std::endl;
		return -1;
	}

	// add the server to the poll

    pollfd srv = {sock_fd, POLLIN, 0};
    _pfds.push_back(srv);

	std::cout << "Server is listening" << std::endl;

	while (_running)
	{
		if (poll(_pfds.begin().base(), _pfds.size(), -1) < 0)
		{
			std::cerr << "Error while polling from fd!" << std::endl;
			return -1;
		}

		//	one or more descriptors are readable => process them

		for (pfd_iterator it = _pfds.begin(); it != _pfds.end(); it++)
		{
			if (it->revents == 0)
				continue;

			if ((it->revents & POLLHUP) == POLLHUP)
			{
				//on_client_disconnect(it->fd);
				break;
			}

			if ((it->revents & POLLIN) == POLLIN)
			{
				if (it->fd == sock_fd)
				{
					std::cout << "Connect" << std::endl;
					client_Connect(sock_fd, _pfds);
					break;
				}

				//on_client_message(it->fd);
			}
		}
	}
	return 0;
}