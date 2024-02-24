#include "../include/ServerHandler.hpp"

ServerHandler::ServerHandler()
{
}


ServerHandler::~ServerHandler()
{
	if (!_clients.empty())
	{
		for (auto client : _clients)
		{
			if (client.second != nullptr)
				delete client.second;
		}
		_clients.clear();
	}

	if (!_servers.empty())
		_servers.clear();

	if (!_pollfds.empty())
		_pollfds.clear();
}

void ServerHandler::initServers(std::map<std::string, Server*> &servers)
{
	for (auto& serverPair : servers)
	{
		std::pair<std::map<std::string, Server *>::iterator, bool> result;
		result = this->_servers.insert(std::pair<std::string, Server *>(serverPair.first, serverPair.second));
		if (!result.second)
			throw ConfigurationException("Duplicate server names used");

		Server* server = this->_servers.find(serverPair.first)->second;
		if (server != nullptr)
		{
			if (server->getClientMaxBodySize() <= 0 || server->getClientMaxBodySize() > MESSAGE_BUFFER)
				server->setClientMaxBodySize(MESSAGE_BUFFER);
			server->setSocket();
			addNewPoll(server->getSocket()->getFd());
		}
	}
}

void ServerHandler::validateServers(std::map<std::string, Server *> &servers)
{
	for (auto& serverPair : servers)
	{
		if (serverPair.second->getName().empty())
			throw ConfigurationException("Server is missing a name");
		if (serverPair.second->getHostIp().empty())
			throw ConfigurationException("Server is missing IP address");
		if (serverPair.second->getListenPort() == 0)
			throw ConfigurationException("Server is missing port");
		if (serverPair.second->getLocationCount() <= 0)
			throw ConfigurationException("Server is missing locations");
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

bool ServerHandler::hasTimedOut(std::chrono::high_resolution_clock::time_point start, int milliseconds)
{
	std::chrono::high_resolution_clock::time_point now = std::chrono::high_resolution_clock::now();
	std::chrono::duration<int> difference = std::chrono::duration_cast<std::chrono::duration<int> >(now - start);

	if (difference.count() >= milliseconds)
		return true;
	return false;
}

void ServerHandler::addNewPoll(int fd)
{
	_pollfds.push_back({fd, POLLIN, 0});
}

void ServerHandler::handleNewClient(Socket *socket)
{
	int newClientFd = -1;
	while (1)
	{
		newClientFd = socket->acceptConnection(socket->getFd());
		if (newClientFd < 0)
			break ;
		addNewPoll(newClientFd);
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
		throw InternalException("Could not send response to client");
}

bool ServerHandler::incomingClient(int fd)
{
	for (std::pair<const std::string, Server*> &pair : this->_servers)
	{
		Server* server = pair.second;
		Socket* socket = server->getSocket();
		if (fd == socket->getFd())
		{
			handleNewClient(socket);
			return true;
		}
	}
	return false;
}

Server *ServerHandler::getServer(HttpRequest *request)
{
	if (request == nullptr)
		return Server::getServer("", this->_servers);
	else
		return Server::getServer(request->getServerName(), this->_servers);
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

	if (client->getServer() == nullptr)
		client->setServer(getServer(nullptr));
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

		requestHandler.handleRequest(it->second, it->second->getServer());
		fd.events = POLLOUT;
	}
}

void ServerHandler::handleIncomingRequest(pollfd *fd)
{
	std::string requestString = readRequest(fd->fd, MESSAGE_BUFFER);
	if (requestString.empty())
		return;

	Client *client = getOrCreateClient(fd);
	if (client->getStatus() == Client::STATUS::NONE)
	{
		HttpRequestParser requestParser;
		HttpRequest *request = requestParser.parseHttpRequest(requestString, this->_servers);
		client->setRequest(request);
		client->setServer(getServer(request));
	}
	else if (client->getStatus() == Client::STATUS::INCOMING)
		client->appendRequest(requestString);

	client->updateStatus();
}

void ServerHandler::handleOutgoingResponse(pollfd *fd)
{
	std::map<int, Client*>::iterator it = _clients.find(fd->fd);
	if (it == _clients.end())
		return;
	writeResponse(fd->fd, HttpResponseParser::Parse(*it->second->getResponse()));
	it->second->setStatus(Client::STATUS::NONE);

	bool close = it->second->closeConnection();
	delete it->second;
	_clients.erase(fd->fd);
	if (close)
		closeConnection(fd->fd);
}

void ServerHandler::handleOutgoingError(const Exception& e, pollfd *fd)
{
	Client *client = getOrCreateClient(fd);
	client->setCloseConnection(true);
	HttpRequestHandler handler;
	client->setResponse(handler.parseErrorResponse(client->getServer(), ExceptionManager::getErrorStatus(e)));
	fd->events = POLLOUT;
}

void ServerHandler::handlePollEvents()
{
	for (unsigned long i = 0; i < _pollfds.size(); i ++)
	{
		if (_pollfds[i].revents == 0)
			continue;
		if (incomingClient(_pollfds[i].fd))
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
	closeConnection(client->first);
	return _clients.erase(client);
}

void ServerHandler::removeTimedOutClients()
{
	for (auto itc = _clients.begin(); itc != _clients.end(); )
	{
		if (hasTimedOut(itc->second->getRequestStart(), CLIENT_TIMEOUT) && itc->second->getStatus() != Client::STATUS::READY_TO_HANDLE)
		{
			std::cout << "Removed timed out request " << itc->first << std::endl;
			itc = removeClient(itc);
		}
		else
			itc++;
		if (itc == _clients.end() || _clients.empty())
			break;
	}
}

void ServerHandler::runServers(std::map<std::string, Server*> &servers)
{
	initServers(servers);
	validateServers(servers);
	while (true)
	{
		removeTimedOutClients();
		// Wait max 3 minutes for incoming traffic
		int result = poll(_pollfds.data(), _pollfds.size(), CONNECTION_TIMEOUT);
		if (result < 0)
		{
			closeConnections();
			throw PollException("Poll failed");
		}
		if (result > 0)
			handlePollEvents();
		handleReadyToBeHandledClients();
	}

	closeConnections();
}
