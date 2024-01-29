
#include <cstdlib> // For exit() and EXIT_FAILURE
#include "../include/ServerHandler.hpp"
#include "../include/ConfigParser.hpp"

// Server& initServer()
// {
// 	Server *server = new Server();
// 	server->setHostIp("127.0.0.1");
// 	server->setListenPort(8000);
// 	server->setName("localhost");
// 	server->setClientMaxBodySize(1000);
// 	server->setSocket();

// 	return *server;
// }

int main(int argc, char **argv)
{
	std::string configFile;

	if (argc != 2 || !argv[1])
		configFile = "config/good/default.conf";
	else
		configFile = argv[1];

	try
	{
		ConfigParser configParse;
		configParse.parseConfig(configFile);
		std::vector<Server *> servers =  configParse.getServers();

		ServerHandler serverHandler;

		serverHandler.runServers(servers);
		servers.clear();
	}
	catch(const std::logic_error& e)
	{
		std::cerr << e.what() << '\n';
	}
	// }
	// else
	// try
	// {
	// 	Server server = initServer();

	// 	ServerHandler serverHandler;
	// 	serverHandler.addNewPoll(server.getSocket()->getFd());

	// 	std::vector<Server> servers;
	// 	servers.push_back(server);
	// 	serverHandler.runServers(servers);

	// 	servers.clear();
	// }
	// catch(const std::logic_error& e)
	// 	{
	// 		std::cerr << e.what() << '\n';
	// 	}
	return 0;
}
