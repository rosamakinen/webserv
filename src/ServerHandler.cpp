
#include "../include/ServerHandler.hpp"

ServerHandler::ServerHandler()
{
}


ServerHandler::~ServerHandler()
{
	if (!_clients.empty())
	{
		for (std::map<int, Client *>::iterator it = _clients.begin(); it != _clients.end(); it++)
			delete it->second;
		_clients.clear();
	}

	if (!_pollfds.empty())
		_pollfds.clear();
}

void ServerHandler::initServers(std::vector<Server*>& servers)
{
	for (Server* server : servers)
	{
		if (server != nullptr)
		{
			if (server->getClientMaxBodySize() == 0)
				server->setClientMaxBodySize(MESSAGE_BUFFER);
			server->setSocket();
			addNewPoll(server->getSocket()->getFd());
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

void ServerHandler::addNewPoll(int fd)
{
	_pollfds.push_back({fd, POLLIN, 0});
}

void ServerHandler::handleNewClient(Socket *socket)
{
	int newClientFd = -1;
	while (1)
	{
		newClientFd = socket->acceptConnection();
		if (newClientFd < 0)
			break ;
		addNewPoll(newClientFd);
	}
}

void ServerHandler::closeConnections()
{
	for (std::vector<pollfd>::iterator it = _pollfds.begin(); it != _pollfds.end(); it++)
	{
		if (it->fd > 0)
		{
			close(it->fd);
			it->fd = -1;
		}
		_pollfds.erase(it);
	}
}

void ServerHandler::closeConnection(int fd)
{
	for (std::vector<pollfd>::iterator it = _pollfds.begin(); it != _pollfds.end(); it++)
	{
		if (fd == it->fd)
		{
			if (it->fd > 0)
			{
				close(it->fd);
				it->fd = -1;
			}
			_pollfds.erase(it);
			break;
		}
	}
}

std::string ServerHandler::readRequest(int connection, unsigned int buffer_size)
{
	char buffer[buffer_size];
	std::string input;

	while (1)
	{
		int readBytes = recv(connection, buffer, sizeof(buffer), 0);
		if (readBytes < 0)
			break;
		if (readBytes == 0)
		{
			closeConnection(connection);
			break;
		}
		buffer[readBytes] = '\0';
		input.append(buffer);
	}

	return input;
}

void ServerHandler::writeResponse(int connection, const std::string response)
{
	int result = send(connection, response.c_str(), response.size(), 0);
	if (result < 0)
		throw InternalException("Could not send response");
}

bool ServerHandler::incomingClient(int fd, std::vector<Server*> &servers)
{
	for (std::vector<Server*>::iterator it = servers.begin(); it != servers.end(); it++)
	{
		Socket *socket = (*it)->getSocket();
		if (fd == socket->getFd())
		{
			handleNewClient(socket);
			return true;
		}
	}

	return false;
}

void ServerHandler::handleIncomingRequest(pollfd *fd)
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

	std::string requestString = readRequest(fd->fd, MESSAGE_BUFFER);
	HttpRequestParser requestParser;
	HttpRequest *request = requestParser.parseHttpRequest(requestString);
	client->setRequest(request);

	// TODO: separate to handler part
	HttpRequestHandler requestHandler;
	requestHandler.handleRequest(client);
	fd->events = POLLOUT;
}

void ServerHandler::handleOutgoingResponse(pollfd *fd)
{
	std::map<int, Client*>::iterator it = _clients.find(fd->fd);
	if (it == _clients.end())
		return;
	writeResponse(fd->fd, HttpResponseParser::Parse(*it->second->getResponse()));

	delete it->second;
	_clients.erase(it);
}

void ServerHandler::handleOutgoingError(const Exception& e, pollfd *fd)
{
	HttpResponse *response = new HttpResponse(ExceptionManager::getErrorStatus(e), "");
	std::map<int, Client*>::iterator it = _clients.find(fd->fd);
	if (it != _clients.end())
	{
		it->second->setResponse(response);
		fd->events = POLLOUT;
		return;
	}

	Client *client = new Client();
	client->setResponse(response);
	std::pair<std::map<int, Client*>::iterator, bool> result;
	result = _clients.insert(std::pair<int, Client*>(fd->fd, client));
	if (!result.second)
		throw BadRequestException("Connection already established with this client");
	fd->events = POLLOUT;
}

void ServerHandler::handlePollEvents(std::vector<Server*>& servers)
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

void ServerHandler::runServers(std::vector<Server*>& servers)
{
	initServers(servers);
	while (1)
	{
		// Wait max 3 minutes for incoming traffic
		int result = poll(_pollfds.data(), _pollfds.size(), CONNECTION_TIMEOUT);
		if (result == 0)
		{
			closeConnections();
			_pollfds.clear();
			throw TimeOutException("The program excited with timeout");
		}
		else if (result < 0)
		{
			closeConnections();
			_pollfds.clear();
			throw PollException("Poll failed");
		}
		handlePollEvents(servers);
	}

	closeConnections();
	_pollfds.clear();
}
