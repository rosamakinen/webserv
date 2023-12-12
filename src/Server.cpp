
#include "../include/Server.hpp"

Server::Server()
{
}

Server::~Server()
{
}

bool	Server::isErrorPage(std::string error) const
{
	if (this->_errorPages.find(error) != _errorPages.end())
		return true;
	return false;
}

bool	Server::isKeyInLocation(std::string location, std::string key) const
{
	if (this->getLocationValue(location, key))
		return true;
	return false;
}

bool	Server::isLocationInServer(std::string location) const
{
	for (locationMap::const_iterator i = this->_location.begin(); i != this->_location.end(); i++)
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

//Setters
void	Server::setServerName(std::string serverName)
{
	this->_serverName = serverName;
}

void	Server::setListenPort(size_t listenPort)
{
	this->_listenPort = listenPort;
}

void	Server::setHostIp(std::string hostIp)
{
	this->_hostIp = hostIp;
}

void	Server::setRoot(std::string rootDir)
{
	this->_rootDir = rootDir;
}

void	Server::setIndex(std::string index)
{
	this->_index = index;
}

void	Server::setClientMaxBodySize(size_t clientMaxBodySize)
{
	this->_clientMaxBodySize = clientMaxBodySize;
}

void	Server::setErrorPage(std::string errorCode, std::string errorPage)
{
	this->_errorPages[errorCode] = errorPage;
}

void	Server::setLocation(std::string key, vectorMap locationValues)
{
	(this->_location)[key] = locationValues;
}

void	Server::setKeyValue(
			std::string location,
			std::string key,
			std::vector<std::string> values)
{
	locationMap::iterator outerIter = this->_location.find(location);
	if (outerIter != this->_location.end())
	{
		vectorMap& inner = outerIter->second;
		vectorMap::iterator innerIter = inner.find(key);
		if (innerIter != inner.end())
			innerIter->second = values;
		else
			inner[key] = values;
	}
	else
	{
		vectorMap newInner;
		newInner[key] = values;
		this->_location[location] = newInner;
	}
}


//Getters
std::string	Server::getServerName() const
{
	return this->_serverName;
}

size_t	Server::getListenPort() const
{
	return this->_listenPort;
}

std::string	Server::getHostIp() const
{
	return this->_hostIp;
}

std::string	Server::getRoot() const
{
	return this->_rootDir;
}

std::string	Server::getIndex() const
{
	return this->_index;
}

size_t	Server::getClientMaxBodySize() const
{
	return this->_clientMaxBodySize;
}

std::string	Server::getErrorPage(std::string error) const
{
	return this->_errorPages.find(error)->second;
}

struct sockaddr_in	Server::getAddress() const
{
	return this->_address;
}

size_t	Server::getLocationCount() const
{
	return this->_location.size();
}

size_t	Server::getLocationCount(std::string location) const
{
	size_t	count = 0;
	locationMap::const_iterator outerIter = this->_location.find(location);
	if (outerIter != this->_location.end())
	{
		const vectorMap& inner = outerIter->second;
		for (vectorMap::const_iterator i = inner.begin(); i != inner.end(); i++)
			count++;
	}
	return count;
}

const std::vector<std::string>*	Server::getLocationValue(std::string location, std::string key) const
{
	locationMap::const_iterator outerIter = this->_location.find(location);
	if (outerIter != this->_location.end())
	{
		const vectorMap& inner = outerIter->second;
		vectorMap::const_iterator innerIter = inner.find(key);
		if (innerIter != inner.end())
			return &(innerIter->second);
	}
	return NULL;
}
