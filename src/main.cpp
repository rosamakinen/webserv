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

void addNewPoll(std::vector<pollfd>& fds, int fd)
{
	fds.push_back({fd, POLLIN, 0});
}

void	handleNewClient(Socket *socket, std::vector<pollfd>& fds)
{
	int newClientFd = -1;
	while (1)
	{
		newClientFd = socket->acceptConnection();
		if (newClientFd < 0)
			break ;
		addNewPoll(fds, newClientFd);
	}
}

void closeConnections(std::vector<pollfd> fds)
{
	for (std::vector<pollfd>::iterator it = fds.begin(); it != fds.end(); it++)
	{
		if (it->fd > 0)
		{
			close(it->fd);
			it->fd = -1;
		}
		fds.erase(it);
	}
}

void runServer(Server *server)
{
	Socket *socket = new Socket(server->getListenPort());

	// Initialize poll struct for sockets and clients
	int socketFd = socket->getFd();
	// Add socket fd to pollfds to listen to connections
	std::vector<pollfd> pollfds;
	addNewPoll(pollfds, socketFd);

	bool keepRunning = true;
	std::string requestString;
	while (keepRunning)
	{
		// Wait max 3 minutes for incoming traffic
		int result = poll(pollfds.data(), pollfds.size(), CONNECTION_TIMEOUT);
		if (result == 0)
			throw TimeOutException("The program excited with timeout");
		else if (result < 0)
			throw PollException("Poll failed");

		for (unsigned long i = 0; i < pollfds.size(); i ++)
		{
			if (pollfds[i].revents == 0)
				continue;
			if (pollfds[i].fd == socketFd)
				handleNewClient(socket, pollfds);
			else if (pollfds[i].revents == POLLIN)
			{
				try
				{
					requestString = socket->readRequest(pollfds[i].fd, server->getClientMaxBodySize());
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

					socket->writeResponse(pollfds[i].fd, HttpResponseParser::Parse(response, server));
				}
				catch (const Exception& e)
				{
					HttpResponse response(ExceptionManager::getErrorStatus(e), "");
					socket->writeResponse(pollfds[i].fd, HttpResponseParser::Parse(response, server));
				}
			}
		}
	}

	closeConnections(pollfds);
	pollfds.clear();
	delete socket;
}

// Candidate for removal after testing
void printVector(const std::vector<std::string>* vecPtr)
{
	if (vecPtr)
	{
		for (std::vector<std::string>::const_iterator it = vecPtr->begin(); it != vecPtr->end(); ++it)
		{
			std::cout << *it;
			if (it + 1 != vecPtr->end())
				std::cout << ", ";
		}
		std::cout << std::endl;
	}
}

void dansTestFunc()
{
	ConfigParser parser;
	parser.parseConfig("config/default.conf");

	// Access the servers
	const std::vector<std::shared_ptr<Server>>& servers = parser.getServers();
	for (std::vector<std::shared_ptr<Server>>::const_iterator it = servers.begin(); it != servers.end(); ++it) {
		std::shared_ptr<Server> server = *it;
		if (server)
		{
			std::cout << "Methods: ";
			printVector(server->getLocationValue("/", "method"));
			printVector(server->getLocationValue("/tmp", "method"));
			printVector(server->getLocationValue("/cgi-bin", "directory"));
		}
	}
}

int main(int argc, char **argv)
{
	if (argc != 2 || !argv[1])
	{
		dansTestFunc();
		return 0;
	}

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
