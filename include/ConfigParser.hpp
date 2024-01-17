#pragma once

#include "../include/WebServer.hpp"
#include "../include/Server.hpp"

class ConfigParser
{
private:
	std::vector<std::string> sectionStack;
	std::string currentSection;
	std::vector<std::shared_ptr<Server> > servers;
	std::shared_ptr<Server> currentServer;
	vectorMap vStack;
	std::string currentLocation;

	void processLine(const std::string &line);

public:
	void parseConfig(const std::string& filename);
	const std::vector<std::shared_ptr<Server> >& getServers() const;
	void clearMap(vectorMap& vMap);
};

