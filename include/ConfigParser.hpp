#pragma once

#include "../include/WebServer.hpp"
#include "../include/Server.hpp"

class ConfigParser
{
private:
	std::vector<Server> servers;
	std::vector<std::string> sectionStack;
	std::string currentSection;
	std::string currentLocation;
	size_t lineNumber;
	vectorMap vStack;

	void checkServer();
	void checkMain(const std::string& keyword, const std::string& value);
	void processLine(const std::string &line);

public:
	ConfigParser();
	~ConfigParser();

	void parseConfig(const std::string& filename);
	const std::vector<Server>& getServers() const;
};

