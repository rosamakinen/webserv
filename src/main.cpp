#include <cstdlib> // For exit() and EXIT_FAILURE
#include <iostream> // For cout
#include <unistd.h> // For read
#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>

#include "../include/Socket.hpp"
#include "../include/Server.hpp"

#define TIMEOUT 180000

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

pollfd	*addNewPoll(pollfd *fds, int size, int fd, short events)
{
	struct pollfd *newFds = new pollfd[size + 1];

	if (fds != NULL)
	{
		for (int i = 0; i < size; i++)
			newFds[i] = fds[i];
		delete [] fds;
	}
	newFds[size].fd = fd;
	newFds[size].events = events;

	return newFds;
}

void	handleNewClient(int *numberOfFds, Socket *socket, pollfd **fds)
{
	int newClientFd = -1;
	while (1)
	{
		newClientFd = socket->acceptConnection();
		if (newClientFd < 0)
			break;
		*fds = addNewPoll(*fds, *numberOfFds, newClientFd, POLLIN);
		*numberOfFds += 1;
	}
}

int main()
{
	Server *server = initServer();
	Socket *socket = new Socket(server->getPort());

	// Initialize poll struct for sockets and clients
	int numberOfFds = 1, currentFdsSize = 0, socketFd = socket->getFd();
	struct pollfd *fds = NULL;
	fds = addNewPoll(fds, 0, socketFd, POLLIN);
	while (1)
	{
		// Wait max 3 minutes for incoming traffic
		int result = poll(fds, numberOfFds, TIMEOUT);
		if (result <= 0)
			break;

		currentFdsSize = numberOfFds;
		std::string request;
		for (int i = 0; i < currentFdsSize; i ++)
		{
			if (fds[i].revents == 0)
				continue;
			if (fds[i].revents != POLLIN)
				break; // TODO this is an error?
			if (fds[i].fd == socketFd)
				handleNewClient(&numberOfFds, socket, &fds);
			else
			{
				request = socket->readRequest(fds[i].fd, server->getRequestMaxBodySize());
				if (request.compare("Q\r\n") == 0)
					break;
				// TODO: handle request
				std::cout << "Request from '" << i << "' was: " << request;
				socket->writeResponse(fds[i].fd, "HTTP/1.1 200 OK\r\n");
			}
		}

		if (request.compare("Q\r\n") == 0)
			break;
	}

	socket->closeConnections(fds, currentFdsSize);
	delete [] fds;
	delete socket;
	delete server;

	return 0;
}
