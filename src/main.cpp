#include "../include/WebServer.hpp"
#include "../include/ServerHandler.hpp"
#include "../include/ConfigParser.hpp"

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
		std::map<std::string, Server*> servers = configParse.getServers();

		ServerHandler serverHandler;

		serverHandler.runServers(servers);
		servers.clear();
	}
	catch(const std::logic_error& e)
	{
		std::cerr << e.what() << '\n';
	}

	return 0;
}
