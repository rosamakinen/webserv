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
	const std::map<std::string, Server*>& getServers() const;

private: 
	std::map<std::string, Server*> servers;
	std::vector<std::string> sectionStack;
	std::string currentSection;
	std::string currentLocation;
	size_t lineNumber;
	vectorMap vStack;
	Server* temporaryServer;

	static std::vector<int> validErrorStatusCodes;

	Server* checkServer();
	void checkMain(const std::string& keyword, const std::string& value, const std::string path);
	bool invalidErrorPageConfig(int status, std::string path);
	void processLine(const std::string &line);
	void configError(const std::string &str, size_t lineNumber);
};

#endif
