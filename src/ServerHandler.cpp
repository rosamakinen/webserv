#include "../include/ServerHandler.hpp"

ServerHandler::ServerHandler()
{
}


ServerHandler::~ServerHandler()
{
	if (!_clients.empty())
		_clients.clear();

	if (!_pollfds.empty())
		_pollfds.clear();

	if (!_serverPolls.empty())
		_serverPolls.clear();
}

void ServerHandler::initServers(std::map<std::string, Server*> &servers)
{
	for (const auto& serverPair : servers)
	{
		Server* server = serverPair.second;
		if (server != nullptr)
		{
			if (server->getClientMaxBodySize() <= 0)
				server->setClientMaxBodySize(MESSAGE_BUFFER);
			server->setSocket();
			addNewPoll(server, server->getSocket()->getFd(), false);
		}
	}
}

void ServerHandler::isCallValid(const int fd, const std::string errorMsg, int closeFd)
{
	if (fd < 0)
	{
		if (closeFd != -1)
			close(closeFd);
		throw PollException(errorMsg);
	}
}

bool ServerHandler::hasTimedOut(Client *client)
{
	std::chrono::high_resolution_clock::time_point now = std::chrono::high_resolution_clock::now();
	std::chrono::duration<int> difference = std::chrono::duration_cast<std::chrono::duration<int> >(now - client->getRequestStart());

	if (difference.count() >= 10)
	{
		std::cout << "The client with the request '" << client->getRequest()->getBody() << "' has timed out after " << difference.count() << " milliseconds." << std::endl;
		return true;
	}
	return false;
}

void ServerHandler::addNewPoll(Server *server, int fd, bool addServer)
{
	_pollfds.push_back({fd, POLLIN, 0});
	if (addServer)
		_serverPolls.insert(std::pair<int, Server*>(fd, server));
}

void ServerHandler::handleNewClient(Socket *socket, Server *server)
{
	int newClientFd = -1;
	while (1)
	{
		newClientFd = socket->acceptConnection();
		if (newClientFd < 0)
			break ;
		addNewPoll(server, newClientFd, true);
	}
}

void ServerHandler::closeConnections()
{
	std::vector<pollfd>::iterator it;
	for (it = _pollfds.begin(); it != _pollfds.end(); )
	{
		if (it->fd > 0)
		{
			close(it->fd);
			it->fd = -1;
		}
		it = _pollfds.erase(it);
	}

	_pollfds.clear();
}

void ServerHandler::closeConnection(int fd)
{
	for (auto it = _pollfds.begin(); it != _pollfds.end(); )
	{
		if (fd != it->fd)
		{
			++it;
			continue;
		}

		if (it->fd >  0)
		{
			close(it->fd);
			it->fd = -1;
		}

		_serverPolls.erase(it->fd);
		it = _pollfds.erase(it);
	}
}

std::string ServerHandler::readRequest(int connection, unsigned int buffer_size)
{
	char buffer[buffer_size];
	std::string input;

	int readBytes = recv(connection, buffer, sizeof(buffer), 0);
	if (readBytes < 0)
	{
		closeConnection(connection);
		return "";
	}

	if (readBytes == 0)
	{
		closeConnection(connection);
		return "";
	}

	buffer[readBytes] = '\0';
	input.append(buffer);

	return input;
}

void ServerHandler::writeResponse(int connection, const std::string response)
{
	int result = send(connection, response.c_str(), response.size(), 0);
	if (result < 0)
		throw InternalException(" send response");
}

bool ServerHandler::incomingClient(int fd, std::map<std::string, Server*> &servers)
{
	for (std::pair<const std::string, Server*> &pair : servers)
	{
		Server* server = pair.second;
		Socket* socket = server->getSocket();
		if (fd == socket->getFd())
		{
			handleNewClient(socket, server);
			return true;
		}
	}
	return false;
}

Server *ServerHandler::getServer(int fd)
{
	std::map<int, Server*>::iterator it = _serverPolls.find(fd);
	if (it == _serverPolls.end())
		throw InternalException("No such server found");

	return it->second;
}

Client *ServerHandler::getOrCreateClient(pollfd *fd)
{
	Client *client;
	std::map<int, Client*>::iterator it = _clients.find(fd->fd);
	if (it == _clients.end())
	{
		client = new Client();
		std::pair<std::map<int, Client*>::iterator, bool> result;
		result = _clients.insert(std::pair<int, Client*>(fd->fd, client));
		if (!result.second)
			throw BadRequestException("Connection already established with this client");
	}
	else
		client = it->second;

	Server *server = getServer(fd->fd);
	client->setServer(server);

	return client;
}

void ServerHandler::handleReadyToBeHandledClients()
{
	HttpRequestHandler requestHandler;

	for (pollfd& fd : _pollfds)
	{
		std::map<int, Client*>::iterator it = _clients.find(fd.fd);
		if (it == _clients.end() || it->second->getStatus() != Client::STATUS::READY_TO_HANDLE)
			continue;

		std::cout << "Client '" << fd.fd << "' is being handled." << std::endl;
		requestHandler.handleRequest(it->second, it->second->getServer());
		fd.events = POLLOUT;
	}
}

void ServerHandler::handleIncomingRequest(pollfd *fd)
{
	Client *client = getOrCreateClient(fd);
	std::cout << "Reading client " << fd->fd << std::endl;

	std::string requestString = readRequest(fd->fd, MESSAGE_BUFFER);
	if (client->getStatus() == Client::STATUS::NONE)
	{
		HttpRequestParser requestParser;
		HttpRequest *request = requestParser.parseHttpRequest(requestString, client->getServer());
		client->setRequest(request);
	}
	else if (client->getStatus() == Client::STATUS::INCOMING)
	{
		client->appendRequest(requestString);
		std::cout << "Client status: '" << client->getStatus() << "'" << std::endl << std::endl;
	}

	client->updateStatus();
	std::cout << "Client status: '" << client->getStatus() << "'" << std::endl << std::endl;
}

void ServerHandler::handleOutgoingResponse(pollfd *fd)
{
	std::map<int, Client*>::iterator it = _clients.find(fd->fd);
	if (it == _clients.end())
		return;
	writeResponse(fd->fd, HttpResponseParser::Parse(*it->second->getResponse()));
	it->second->setStatus(Client::STATUS::NONE);

	delete it->second;
	_clients.erase(fd->fd);
}

void ServerHandler::handleOutgoingError(const Exception& e, pollfd *fd)
{
	Server *server = getServer(fd->fd);
	Client *client = getOrCreateClient(fd);
	HttpRequestHandler handler;
	client->setResponse(handler.parseErrorResponse(server, ExceptionManager::getErrorStatus(e)));
	fd->events = POLLOUT;
}

void ServerHandler::handlePollEvents(std::map<std::string, Server*> &servers)
{
	for (unsigned long i = 0; i < _pollfds.size(); i ++)
	{
		if (_pollfds[i].revents == 0)
			continue;
		if (incomingClient(_pollfds[i].fd, servers))
			continue;
		else if (_pollfds[i].revents & POLLIN)
		{
			try
			{
				handleIncomingRequest(&_pollfds[i]);
			}
			catch (const Exception& e)
			{
				handleOutgoingError(e, &_pollfds[i]);
			}
		}
		else if (_pollfds[i].revents & POLLOUT)
		{
			try
			{
				handleOutgoingResponse(&_pollfds[i]);
			}
			catch (const Exception& e)
			{
				handleOutgoingError(e, &_pollfds[i]);
			}
		}
	}
}

std::map<int, Client*>::iterator ServerHandler::removeClient(std::map<int, Client*>::iterator client)
{
	delete client->second;

	for (unsigned long i = 0; i < _pollfds.size(); i++)
	{
		if (_pollfds[i].fd != client->first)
			continue;
		closeConnection(_pollfds[i].fd);
	}

	return _clients.erase(client);
}

void ServerHandler::removeTimedOutClients()
{
	for (auto it = _clients.begin(); it != _clients.end(); )
	{
		if (hasTimedOut(it->second) && it->second->getStatus() != Client::STATUS::READY_TO_HANDLE)
			it = removeClient(it);
		else
			it++;
	}
}

void ServerHandler::runServers(std::map<std::string, Server*> &servers)
{
	initServers(servers);
	while (true)
	{
		removeTimedOutClients();
		// Wait max 3 minutes for incoming traffic
		int result = poll(_pollfds.data(), _pollfds.size(), CONNECTION_TIMEOUT);
		if (result == 0)
		{
			closeConnections();
			// TODO: throw Timeout to all clients and close connections, dont shut down the program
			throw TimeOutException("The program excited with timeout");
		}
		else if (result < 0)
		{
			closeConnections();
			throw PollException("Poll failed");
		}
		handlePollEvents(servers);
		handleReadyToBeHandledClients();
	}

	closeConnections();
	_pollfds.clear();
}
