#include <cstdlib> // For exit() and EXIT_FAILURE
#include <iostream> // For cout
#include <unistd.h> // For read
#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>

#include "../include/Socket.hpp"
#include "../include/Server.hpp"
#include "../include/HttpResponseParser.hpp"
#include "../include/Exceptions.hpp"
#include "../include/ExceptionManager.hpp"
#include "../include/HttpRequest.hpp"
#include "../include/HttpRequestParser.hpp"

Server* initServer()
{
	Server *server = new Server();
	server->setHostIp("127.0.0.1");
	server->setListenPort(8000);
	server->setName("localhost");
	server->setRoot("/");
	server->setClientMaxBodySize(1000);

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
		if (result <= 0)
			break;

		currentFdsSize = numberOfFds;
		std::string requestString;
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
				try
				{
					requestString = socket->readRequest(fds[i].fd, server->getClientMaxBodySize());
					if (requestString.compare("Q\r\n") == 0)
					{
						keepRunning = false;
						break ;
					}

					std::cout << "Request from '" << i << "' was: " << requestString;
					HttpRequestParser requestParser;
					HttpRequest request = requestParser.parseHttpRequest(requestString);

					HttpResponse response("text/html; charset=utf-8");
					response.setBody("<!DOCTYPE html>\r\n<html lang=\"en\" data-color-mode=\"auto\" data-light-theme=\"light\" data-dark-theme=\"dark_tritanopia\" data-a11y-animated-images=\"system\" data-a11y-link-underlines=\"true\">\r\n<head>\r\n<title>Hello World!</title>\r\n</head>\r\n<body>\r\n<h1>Hello, stranger!</h1>\r\n<p>Chrome sent you a request and you answered!</p>\r\n<p>Well done!</p>\r\n</body>\r\n</html>");
					response.setStatus(std::pair<unsigned int, std::string>(200, "OK"));
					socket->writeResponse(fds[i].fd, HttpResponseParser::Parse(response, server));
				}
				catch (const Exception& e)
				{
					HttpResponse response("txt/html");
					response.setStatus(ExceptionManager::getErrorStatus(e));
					socket->writeResponse(fds[i].fd, HttpResponseParser::Parse(response, server));
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
	try
	{
		Server *server = initServer();
		runServer(server);

		delete server;
	}
	catch(const std::logic_error& e)
	{
		std::cerr << e.what() << '\n';
	}

	return 0;
}
