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

    void processLine(const std::string& line);
    std::string trim(const std::string& str);

public:
	void parseConfig(const std::string& filename);
	const std::vector<std::shared_ptr<Server> >& getServers() const;
};

