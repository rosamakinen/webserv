#pragma once

#include <vector>;
#include "Server.hpp";

class Server;

class ConfigParser
{
	private:
		std::vector<Server>			servers;
		std::vector<std::string>	serverConfig;
		size_t						serverNumber;

	public:
		ConfigParser();
		~ConfigParser();


};
