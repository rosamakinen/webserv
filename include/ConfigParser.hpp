#pragma once

#include "../include/WebServer.hpp"

class ConfigParser
{
private:
	std::string currentSection;
	std::vector<std::string> sectionStack;

	void processLine(const std::string& line);
	std::string trim(const std::string& str);

public:
	void parseConfig(const std::string& filename);
};
