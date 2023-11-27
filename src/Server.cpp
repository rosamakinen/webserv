
#include "../include/Server.hpp"
#include "Server.hpp"

Server::Server(void) : ip("127.0.0.1"), port(8000)
{
}

Server::Server(const std::string &ip, const unsigned int &port) : ip(ip), port(port), server_name(""), request_max_body_size(1000)
{
}

Server::~Server(void)
{
	if (!this->locations.empty())
	{
		// TODO: clear location directory paths map && methods
		std::vector<Server::location>::iterator it = this->locations.begin();
		for (; it < this->locations.end(); it++)
		{
			it->methods.clear();
			it->directory_pairs.clear();
		}
		this->locations.clear();
	}
}

void Server::copyLocations(const std::vector<location> locations)
{
	std::vector<const Server::location>::iterator it = locations.begin();
	for (; it < locations.end(); it++)
		this->locations.push_back(*it);
}

Server::Server(const Server &rhs) : ip(rhs.ip), port(rhs.port), server_name(rhs.server_name), request_max_body_size(rhs.request_max_body_size)
{
	this->copyLocations(rhs.locations);
}

Server &Server::operator=(const Server &rhs)
{
	if (this != &rhs)
	{
		this->server_name = rhs.server_name;
		this->request_max_body_size = rhs.request_max_body_size;
		this->copyLocations(rhs.locations);
	}

	return *this;
}

void Server::setServerName(const std::string server_name)
{
	this->server_name = server_name;
}

void Server::setRequestMaxBodySize(const unsigned int request_max_body_size)
{
	this->request_max_body_size = request_max_body_size;
}

void Server::copyLocationMethods(const std::vector<std::string> methods, Server::location *location)
{
	std::vector<const std::string>::iterator it = methods.begin();
	for (; it < methods.end(); it++)
		location->methods.push_back(*it);
}

void Server::addDirectoryPair(const std::string key, const std::string value, struct location *location)
{
	location->directory_pairs.insert(std::pair<std::string, std::string>(key, value));
}

void Server::addLocation(const std::string root, const std::vector<std::string> methods)
{
	location l;
	l.root = root;
	this->copyLocationMethods(methods, &l);
	this->locations.push_back(l);
}
