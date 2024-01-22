#include "../include/ConfigParser.hpp"
#include "../include/Server.hpp"

ConfigParser::ConfigParser()
{
}

ConfigParser::~ConfigParser()
{
}

static void configError(const std::string& str, const std::string& lineNumber)
{
	std::string base = "Config File Error: ";
	base += str + " Line: " + lineNumber;
	throw ConfigurationException(base);
}

static bool validIp(const std::string& ip)
{
	std::stringstream ss(ip);
	std::string segment;
	int segmentCount = 0;
	int segmentValue;

	while (std::getline(ss, segment, '.'))
	{
		segmentCount++;
		if (!(std::istringstream(segment) >> segmentValue) || segmentValue < 0 || segmentValue > 255)
			return false;
	}
	return segmentCount == 4;
}

void ConfigParser::clearMap(vectorMap& vMap)
{
	for (vectorMap::iterator it = vMap.begin(); it != vMap.end(); ++it) {
		it->second.clear();
	}
	vMap.clear();
}

static std::string trim(const std::string& str)
{
	size_t first = str.find_first_not_of(WHITESPACE);
	if (first == std::string::npos)
		return "";
	size_t last = str.find_last_not_of(WHITESPACE);
	return str.substr(first, (last - first + 1));
}

void ConfigParser::parseConfig(const std::string& filename)
{
	std::ifstream file(filename);
	std::string line;
	lineNumber = 1;

	if (!file.is_open())
		configError("Failed to open file.", std::to_string(lineNumber));
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
				else
					configError("Attempting to close unopened block.", std::to_string(lineNumber));
			}
			else
				processLine(line);
		}
		if (currentServer &&  sectionStack.size() == 1 && vStack.size() != 0)
		{
			currentServer->setLocation(currentLocation, vStack);
			clearMap(vStack);
		}
	lineNumber++;
	}
}

const std::vector<std::shared_ptr<Server> > &ConfigParser::getServers() const
{
	return servers;
}

void ConfigParser::checkServer()
{
	if (sectionStack.size() != 0)
		configError("Unclosed block before new server declaration.", std::to_string(lineNumber));
	currentServer = std::make_shared<Server>();
	servers.push_back(currentServer);
}

void ConfigParser::checkMain(const std::string keyword, const std::string value)
{
	if (keyword.compare(PARSEHOST) == 0)
	{
		if (!validIp(value))
			configError("Invalid IP address.", std::to_string(lineNumber));
		currentServer->setHostIp(value);
	}
	else if (keyword.compare(PARSELISTEN) == 0)
		{
			size_t port = std::stol(value);
			if (port > UINT16_MAX || port < 0 || value.empty())
				configError("Invalid port.", std::to_string(lineNumber));
			currentServer->setListenPort(port);
		}
	else if (keyword.compare(PARSENAME) == 0)
		{
			if (value.empty())
				configError("No server name provided.", std::to_string(lineNumber));
			currentServer->setName(value);
		}
	else if (keyword.compare(PARSESIZE) == 0)
		currentServer->setClientMaxBodySize(std::stol(value));
}

void ConfigParser::processLine(const std::string &line)
{
	std::istringstream iss(line);
	std::string keyword;
	iss >> keyword;

	if (keyword.compare(SERVERBLOCK) == 0)
		checkServer();
	if (keyword.compare(MAINBLOCK) == 0 || keyword.compare(LOCATIONBLOCK) == 0)
	{
		if (sectionStack.size() != 1)
			configError("Main and location blocks must be a direct child of server.", std::to_string(lineNumber));
		currentSection = keyword;
	}
	if (currentSection.compare(MAINBLOCK) == 0)
	{
		std::string value;
		iss >> value;;
		if (currentServer)
			checkMain(keyword, value);
	}
	if (keyword.compare(LOCATIONBLOCK) == 0)
		iss >> currentLocation;
	else if (currentServer && currentSection.compare(LOCATIONBLOCK) == 0)
			currentServer->addToVectorMap(vStack, line);
}
