#pragma once

#include "../include/Server.hpp"

class ServerManager
{
	private:
		std::vector<Server> servers;
		std::map<int, Server> serversMap;

	public:
		ServerManager();
		~ServerManager();
};
