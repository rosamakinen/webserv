
#include "../include/Server.hpp"

Server::Server()
{
}

Server::~Server()
{
	if (this->_socket)
		delete this->_socket;
	this->_locations.clear();
}

Server::Server( std::string serverName,	size_t listenPort, std::string hostIp,
	std::string clientMaxBodySize) : _name(serverName), _listenPort(listenPort), _hostIp(hostIp)
{
	if (!clientMaxBodySize.empty())
		this->setClientMaxBodySize(std::stol(clientMaxBodySize));
	else
		this->setClientMaxBodySize(MESSAGE_BUFFER);
}

bool	Server::isKeyInLocation(std::string location, std::string key) const
{
	if (this->getLocationValue(location, key))
		return true;
	return false;
}

bool	Server::isLocationInServer(std::string location) const
{
	for (locationMap::const_iterator i = this->_locations.begin(); i != this->_locations.end(); i++)
		if (!location.compare(i->first))
			return true;
	return false;
}

bool	Server::isValueForKey(std::string location, std::string key, std::string value) const
{
	const std::vector<std::string>* values = this->getLocationValue(location, key);
	if (!values || values->empty())
		return false;
	else if (std::find(values->begin(), values->end(), value) == values->end())
		return false;
	return true;
}

void	Server::setName(std::string serverName)
{
	this->_name = serverName;
}

void Server::setSocket()
{
	this->_socket = new Socket(this->getListenPort());
}
void Server::setListenPort(size_t listenPort)
{
	this->_listenPort = listenPort;
}

void	Server::setHostIp(std::string hostIp)
{
	this->_hostIp = hostIp;
}

void	Server::setClientMaxBodySize(size_t clientMaxBodySize)
{
	this->_clientMaxBodySize = clientMaxBodySize;
}

void	Server::setLocation(std::string key, vectorMap locationValues)
{
	std::pair<locationMap::iterator, bool> result;
	result = _locations.insert(std::pair<std::string, vectorMap>(key, locationValues));
	if (result.second == false)
		throw ConfigurationException("Location already exists");
}

void	Server::addToVectorMap(vectorMap &vMap, std::string line)
{
	std::stringstream ss(line);
	std::istream_iterator<std::string> begin(ss);
	std::string key = *begin;
	begin++;
	std::istream_iterator<std::string> end;
	std::vector<std::string> vstrings(begin, end);

	vMap.insert(std::pair<std::string, std::vector<std::string> >(key, vstrings));
}

bool Server::addErrorPage(int status, std::string index)
{
	std::pair<std::map<int, std::string>::iterator, bool> result;
	result = this->_errorpages.insert(std::pair<int, std::string>(status, index));
	return result.second;
}

//Getters
std::string	Server::getName() const
{
	return this->_name;
}

Socket *Server::getSocket() const
{
	return this->_socket;
}

size_t Server::getListenPort() const
{
	return this->_listenPort;
}

std::string	Server::getHostIp() const
{
	return this->_hostIp;
}

size_t	Server::getClientMaxBodySize() const
{
	return this->_clientMaxBodySize;
}

size_t	Server::getLocationCount() const
{
	return this->_locations.size();
}

size_t	Server::getLocationCount(std::string location) const
{
	size_t	count = 0;
	locationMap::const_iterator outerIter = this->_locations.find(location);
	if (outerIter != this->_locations.end())
	{
		const vectorMap& inner = outerIter->second;
		for (vectorMap::const_iterator i = inner.begin(); i != inner.end(); i++)
			count++;
	}
	return count;
}

const std::vector<std::string>*	Server::getLocationValue(std::string location, std::string key) const
{
	locationMap::const_iterator outerIter = this->_locations.find(location);
	if (outerIter != this->_locations.end())
	{
		const vectorMap& inner = outerIter->second;
		vectorMap::const_iterator innerIter = inner.find(key);
		if (innerIter != inner.end())
			return &(innerIter->second);
	}

	return nullptr;
}

std::string Server::getErrorPagePath(int status)
{
	std::map<int, std::string>::iterator it = this->_errorpages.find(status);
	if (it == this->_errorpages.end())
		return std::string();
	return it->second;
}
