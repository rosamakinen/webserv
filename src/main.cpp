#include <cstdlib> // For exit() and EXIT_FAILURE
#include <iostream> // For cout
#include <unistd.h> // For read
#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>

#include "../include/Socket.hpp"
#include "../include/Server.hpp"

Server* initServer()
{
	Server *server = new Server("127.0.0.1", 8000);
	server->setName("localhost");
	server->setRequestMaxBodySize(1000);
	std::vector<std::string> methods;
	methods.push_back("GET");
	methods.push_back("POST");
	server->addLocation("/", methods);

	return server;
}

void	isCallValid(const int fd, const std::string errorMsg, int closeFd)
{
	if (fd < 0)
	{
		std::cerr << errorMsg << std::endl;
		if (closeFd != -1)
			close(closeFd);
	}
}

int main()
{
	Server *server = initServer();
	fd_set currentSockets, readySockets;
	FD_ZERO(&currentSockets);

	Socket socket(server->getPort());
	int biggest_fd = socket.getFd();
	FD_SET(biggest_fd, &currentSockets);

	struct timeval timer;
	while (1)
	{
		timer.tv_sec = 1;
		timer.tv_usec = 0;

		// Select incoming connections
		readySockets = currentSockets;
		int selected = select(biggest_fd + 1, &readySockets, NULL, NULL, &timer);
		if (selected < 0)
			break ; // TODO: add error / exit

		for (int fd = 0; fd <= biggest_fd; fd++)
		{
			if (FD_ISSET(fd, &readySockets))
			{
				if (fd == socket.getFd())
				{
					int clientSocketFd = socket.acceptConnection();
					isCallValid(clientSocketFd, "Could not accept the connection", socket.getFd());
					FD_SET(clientSocketFd, &currentSockets);
					if (clientSocketFd > biggest_fd)
						biggest_fd = clientSocketFd;
				}
				else
				{
					std::string request = socket.readRequest(fd, server->getRequestMaxBodySize());
					std::cout << "The message was: " << request;
					std::string response = "HTTP/1.1 200 OK\r\n\r\n";
					socket.writeResponse(fd, response);
				}
			}
		}
	}

	for (int fd = 0; fd <= biggest_fd; fd++)
	{
		FD_CLR(fd, &currentSockets);
		close(fd);
	}

	delete server;

	return 0;
}
