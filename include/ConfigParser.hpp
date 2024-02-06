#pragma once

#include "../include/WebServer.hpp"
#include "../include/Server.hpp"

class ConfigParser
{
public:
	ConfigParser();
	~ConfigParser();

	void parseConfig(const std::string& filename);
	const std::vector<Server *>& getServers() const;

private:
	std::vector<Server *> servers;
	std::vector<std::string> sectionStack;
	std::string currentSection;
	std::string currentLocation;
	int currentStatus;
	size_t lineNumber;
	vectorMap vStack;
	std::pair<int, std::string> currentErrorPagePair;

	void checkServer();
	void checkMain(const std::string& keyword, const std::string& value);
	void processLine(const std::string &line);

};

