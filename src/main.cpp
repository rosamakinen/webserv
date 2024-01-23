
#include <cstdlib> // For exit() and EXIT_FAILURE
#include "../include/ServerHandler.hpp"
#include "../include/ConfigParser.hpp"

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

Server& initServer()
{
	Server *server = new Server();
	server->setHostIp("127.0.0.1");
	server->setListenPort(8000);
	server->setName("localhost");
	server->setClientMaxBodySize(1000);
	server->setSocket();

	return *server;
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
		Server server = initServer();

		ServerHandler serverHandler;
		serverHandler.addNewPoll(server.getSocket()->getFd());

		std::vector<Server> servers;
		servers.push_back(server);
		serverHandler.runServers(servers);

		servers.clear();
	}
	catch(const std::logic_error& e)
	{
		std::cerr << e.what() << '\n';
	}

	return 0;
}
