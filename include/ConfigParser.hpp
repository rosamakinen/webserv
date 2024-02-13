#ifndef CONFIGPARSER_HPP
#define CONFIGPARSER_HPP

#include "WebServer.hpp"
#include "Server.hpp"
#include "FileHandler.hpp"

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
	size_t lineNumber;
	vectorMap vStack;

	static std::vector<int> validErrorStatusCodes;

	void checkServer();
	void checkMain(const std::string& keyword, const std::string& value, const std::string path);
	bool invalidErrorPageConfig(int status, std::string path);
	void processLine(const std::string &line);
};

#endif
