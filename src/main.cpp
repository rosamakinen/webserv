#include <cstdlib> // For exit() and EXIT_FAILURE
#include <sys/time.h>
#include <sys/types.h>

#include "../include/Socket.hpp"
#include "../include/Server.hpp"
#include "../include/HttpResponseParser.hpp"
#include "../include/Exceptions.hpp"
#include "../include/ExceptionManager.hpp"
#include "../include/HttpRequest.hpp"
#include "../include/HttpRequestParser.hpp"
#include "../include/HttpRequestHandler.hpp"
#include "../include/FileHandler.hpp"
#include "../include/ConfigParser.hpp"

Server* initServer()
{
	Server *server = new Server();
	server->setHostIp("127.0.0.1");
	server->setListenPort(8000);
	server->setName("localhost");
	server->setClientMaxBodySize(1000);

	return server;
}

void	isCallValid(const int fd, const std::string errorMsg, int closeFd)
{
	if (fd < 0)
	{
		if (closeFd != -1)
			close(closeFd);
		throw PollException(errorMsg);
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
			break ;
		*fds = addNewPoll(*fds, *numberOfFds, newClientFd, POLLIN);
		*numberOfFds += 1;
	}
}

void runServer(Server *server)
{
	Socket *socket = new Socket(server->getListenPort());

	// Initialize poll struct for sockets and clients
	int numberOfFds = 1, currentFdsSize = 0, socketFd = socket->getFd();
	struct pollfd *fds = NULL;
	// Add socket fd to pollfds to listen to connections
	fds = addNewPoll(fds, currentFdsSize, socketFd, POLLIN);

	bool keepRunning = true;
	while (keepRunning)
	{
		// Wait max 3 minutes for incoming traffic
		int result = poll(fds, numberOfFds, CONNECTION_TIMEOUT);
		if (result == 0)
			throw TimeOutException("The program excited with timeout");
		else if (result < 0)
			throw PollException("Poll failed");

		currentFdsSize = numberOfFds;
		std::string requestString;
		for (int i = 0; i < currentFdsSize; i ++)
		{
			if (fds[i].revents == 0)
				continue;
			if (fds[i].fd == socketFd)
				handleNewClient(&numberOfFds, socket, &fds);
			else if (fds[i].revents == POLLIN)
			{
				try
				{
					requestString = socket->readRequest(fds[i].fd, server->getClientMaxBodySize(), &numberOfFds);
					if (requestString.compare("Q\r\n") == 0)
					{
						keepRunning = false;
						break ;
					}

					std::cout << "Request from '" << i << "' was: " << requestString;
					HttpRequestParser requestParser;
					HttpRequest request = requestParser.parseHttpRequest(requestString);

					HttpRequestHandler requestHandler;
					HttpResponse response = requestHandler.handleRequest(request);

					socket->writeResponse(fds[i].fd, HttpResponseParser::Parse(response, server), &numberOfFds);
				}
				catch (const Exception& e)
				{
					HttpResponse response(ExceptionManager::getErrorStatus(e), "");
					socket->writeResponse(fds[i].fd, HttpResponseParser::Parse(response, server), &numberOfFds);
				}
			}
		}
	}

	socket->closeConnections(fds, currentFdsSize);
	delete [] fds;
	delete socket;
}

int main()
{
	ConfigParser parser;
	parser.parseConfig("config/default.conf");

	// Access the servers
	const std::vector<std::shared_ptr<Server>>& servers = parser.getServers();
	for (std::vector<std::shared_ptr<Server>>::const_iterator it = servers.begin(); it != servers.end(); ++it) {
		std::shared_ptr<Server> server = *it;
		if (server) {
			std::cout << "Methods: " << server->getLocationValue("/", "method") << std::endl;
		}
	}
}
