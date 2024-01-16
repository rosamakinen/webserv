#include "../include/ConfigParser.hpp"
#include "../include/Server.hpp"
#include "ConfigParser.hpp"

void ConfigParser::parseConfig(const std::string& filename) 
{
	std::ifstream file(filename);
	std::string line;

	if (!file.is_open())
		throw ConfigurationException("Failed to open file.");
	std::string currentSection;
	while (std::getline(file, line)) 
	{
		// Ignore comments
		size_t commentPos = line.find('#');
		if (commentPos != std::string::npos) 
			line = line.substr(0, commentPos);
		// Trim whitespace
		line = trim(line);
		if (!line.empty()) 
		{
			if (line.compare("{") == 0) 
				sectionStack.push_back(currentSection); 
			else if (line.compare("}") == 0) 
			{
				if (!sectionStack.empty()) 
				{
					sectionStack.pop_back();
					if (!sectionStack.empty()) 
						currentSection = sectionStack.back();
					else 
						currentSection.clear();
				}
			} 
			else 
			{
				processLine(line);
			}
		}
	}
}

const std::vector<std::shared_ptr<Server> > &ConfigParser::getServers() const
{
	return servers;
}

void ConfigParser::processLine(const std::string &line)
{
	std::istringstream iss(line);
	std::string keyword;
	iss >> keyword;

	if (keyword.compare(SERVERBLOCK) == 0) 
	{
		currentServer = std::make_shared<Server>();
		servers.push_back(currentServer);
	}
	if (keyword.compare(MAINBLOCK) == 0 || keyword.compare(LOCATIONBLOCK) == 0)
	{	
		currentSection = keyword;
		if (keyword.compare(LOCATIONBLOCK) == 0) 
		{
			// vectorMap locationPair;
			std::string locationName;
			iss >> locationName;
			if (currentSection.find("location") != std::string::npos)
				// Server::addToVectorMap(locationPair, line);
				std::cout << line << " this line is inside a location block" << std::endl;
		}
		if (currentSection.compare(MAINBLOCK) == 0)
		{
			std::string value;
			iss >> value;;
			if (currentServer)
			{
				if (keyword.compare(PARSEHOST) == 0)
					currentServer->setHostIp(value);
				else if (keyword.compare(PARSELISTEN) == 0)
					currentServer->setListenPort(std::stol(value));
				else if (keyword.compare(PARSENAME) == 0)
					currentServer->setName(value);
				else if (keyword.compare(PARSESIZE) == 0)
					currentServer->setClientMaxBodySize(std::stol(value));
			}
		}
	}
	// Debugging for Dan, will remove it after putting in the locations.	
	// else
		// std::cout << "Processing '" << line << "' in section '" << currentSection << "'" << std::endl;
}

std::string ConfigParser::trim(const std::string& str) 
{
	size_t first = str.find_first_not_of(WHITESPACE);
	if (first == std::string::npos) 
		return "";
	size_t last = str.find_last_not_of(WHITESPACE);
	return str.substr(first, (last - first + 1));
}
