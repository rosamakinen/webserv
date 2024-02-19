#include "../include/ConfigParser.hpp"

ConfigParser::ConfigParser() : lineNumber(1)
{
}

ConfigParser::~ConfigParser()
{
	servers.clear();
}

static void configError(const std::string& str, size_t lineNumber)
{
	std::string base = "Config File Error: ";
	base.append(str).append(" Line: ").append(std::to_string(lineNumber));
	throw ConfigurationException(base);
}

static std::string trim(const std::string& str)
{
	size_t first = str.find_first_not_of(WHITESPACE);
	if (first == std::string::npos)
		return "";
	size_t last = str.find_last_not_of(WHITESPACE);
	return str.substr(first, (last - first + 1));
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

static std::string adjustPath(const std::string& path)
{
	if (!path.empty() && path[0] == '/')
		return "." + path;
	return "./" + path;
}

static bool directoryExists(const std::string& path)
{
	struct stat info;
	if (stat(path.c_str(), &info) != 0)
		return false;
	return (info.st_mode & S_IFDIR) != 0;
}

static bool checkValidDirectory(const std::string& line, std::string key)
{
	std::istringstream iss(line);
	std::string firstWord;
	iss >> firstWord;

	if (firstWord.compare(key) == 0)
	{
		std::string directoryPath;
		if (iss >> directoryPath)
		{
			std::string adjustedPath = adjustPath(directoryPath);
			return directoryExists(adjustedPath);
		}
	}
	return true;
}

static void uniChecker(const std::vector<Server *> servers)
{
	std::set<std::string> uniqueNames;
	std::set<std::string> uniqueHostPortCombos;

	if (servers.empty())
		throw ConfigurationException("No servers detected.");
	for (const Server* server: servers)
	{
		std::string name = server->getName();
		std::string hostPortCombo = server->getHostIp() + ":" + std::to_string(server->getListenPort());

		if (!uniqueNames.insert(name).second)
			throw ConfigurationException("Duplicate name detected.");
		if (!uniqueHostPortCombos.insert(hostPortCombo).second)
			throw ConfigurationException("Duplicate IP:Port combination detected.");
	}
}

static void clearMap(vectorMap& vMap)
{
	for (vectorMap::iterator it = vMap.begin(); it != vMap.end(); it++)
	{
		it->second.clear();
	}

	vMap.clear();
}


const std::vector<Server *>& ConfigParser::getServers() const
{
	return this->servers;
}

void ConfigParser::checkServer()
{
	if (sectionStack.size() != 0)
		configError("Unclosed block before new server declaration.", lineNumber);
	currentSection.clear();
	currentLocation.clear();
	Server *currentServer = new Server();
	this->servers.push_back(currentServer);
}

std::vector<int> ConfigParser::validErrorStatusCodes =
{
	400,
	403,
	404,
	405,
	500,
	504
};

bool ConfigParser::invalidErrorPageConfig(int status, std::string path)
{
	for (std::vector<int>::iterator it = validErrorStatusCodes.begin(); ; it++)
	{
		if (status == *it)
			break;
		if (it == validErrorStatusCodes.end())
			return false;
	}

	if (path.empty() || path.length() <= 5 || (path.substr(path.length() - 5, path.length()).compare(EXT_HTML) != 0))
		return false;

	std::string fullPath = FileHandler::getFilePath(path);
	struct stat file_status;
	if ((stat(fullPath.c_str(), &file_status) != 0) || S_ISDIR(file_status.st_mode))
		return false;

	return true;
}

void ConfigParser::checkMain(const std::string& keyword, const std::string& value, const std::string path)
{
	if (this->servers.empty())
		configError("No server defined for main block.", lineNumber);

	Server* currentServer = this->servers.back();

	if (keyword.compare(PARSEHOST) == 0)
	{
		if (!validIp(value))
			configError("Invalid IP address.", lineNumber);
		currentServer->setHostIp(value);
	}
	else if (keyword.compare(PARSELISTEN) == 0)
	{
		long port = -1;
		try
		{
			port = std::stol(value);
		}
		catch(const std::logic_error& e)
		{
			configError("Invalid port number.", lineNumber);
		}

		if (port > UINT16_MAX || port < 0 || value.empty())
			configError("Invalid port.", lineNumber);
		currentServer->setListenPort(port);
	}
	else if (keyword.compare(PARSENAME) == 0)
		currentServer->setName(value);
	else if (keyword.compare(PARSESIZE) == 0)
		currentServer->setClientMaxBodySize(std::stol(value));
	else if (keyword.compare(ERRORPAGE_LOCATION) == 0)
	{
		if (value.empty() || path.empty())
			configError("Invalid error page configuration.", lineNumber);
		int status = 0;
		try
		{
			status = std::stoi(value);
		}
		catch(const std::exception& e)
		{
			configError("Invalid status code.", lineNumber);
		}

		if (!invalidErrorPageConfig(status, path))
			configError("Invalid error page configuration.", lineNumber);

		if (!currentServer->addErrorPage(status, path))
			configError("Duplicate error page configuration.", lineNumber);
	}
}

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
		line = trim(line);
		if (!line.empty())
		{
			if (line.compare("{") == 0)
				sectionStack.push_back(currentSection);
			else if (line.compare("}") == 0)
			{
				if (sectionStack.empty())
					configError("Attempting to close unopened block.", lineNumber);
				sectionStack.pop_back();
				if (!sectionStack.empty())
					currentSection = sectionStack.back();
				else
					currentSection.clear();
			}
			else
				processLine(line);
		}
		if (!this->servers.empty())
		{
			Server* currentServer = this->servers.back();
			if (sectionStack.size() == 1 && vStack.size() != 0)
			{
				currentServer->setLocation(currentLocation, vStack);
				clearMap(vStack);
			}
		}
	lineNumber++;
	}
	uniChecker(servers);
}

void ConfigParser::processLine(const std::string &line)
{
	std::istringstream iss(line);
	std::string keyword;
	iss >> keyword;
	Server* currentServer;
	if (!this->servers.empty())
		currentServer = this->servers.back();

	if (keyword.compare(SERVERBLOCK) == 0)
		checkServer();
	if (keyword.compare(MAINBLOCK) == 0 || keyword.compare(LOCATIONBLOCK) == 0)
	{
		if (sectionStack.size() != 1)
			configError("Main, location and error page blocks must be direct children of server.", lineNumber);
		currentSection = keyword;
	}

	if (currentSection.compare(MAINBLOCK) == 0)
	{
		std::string value;
		iss >> value;
		std::string path;
		iss >> path;
		checkMain(keyword, value, path);
	}

	if (keyword.compare(LOCATIONBLOCK) == 0)
	{
		iss >> currentLocation;
		if (currentLocation.empty())
			configError("Unnamed location.", lineNumber);

	}
	else if (currentSection.compare(LOCATIONBLOCK) == 0)
	{
		if (!checkValidDirectory(line, "directory"))
			configError("Directory does not exist.", lineNumber);
		currentServer->addToVectorMap(vStack, line);
	}
}
