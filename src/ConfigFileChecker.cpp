#include "../include/ConfigFileChecker.hpp"

ConfigFileChecker::ConfigFileChecker() : size(0)
{
}

ConfigFileChecker::~ConfigFileChecker()
{
}

ConfigFileChecker::ConfigFileChecker(std::string const path) : path(path), size(0)
{
}

/* Return values: error(0), file(1), dir(2), else(3) */
int ConfigFileChecker::getPathType(std::string const path)
{
	struct stat	buffer;
	int			type;

	type = stat(path.c_str(), &buffer);
	if (type == 0)
	{
		if (buffer.st_mode & S_IFREG)
			return 1;
		else if (buffer.st_mode & S_IFDIR)
			return 2;
		return 3;
	}
	return 0;
}

/* Returns 0 on success, -1 on failure & sets errno*/
int ConfigFileChecker::checkFile(std::string const path, int mode)
{
	return (access(path.c_str(), mode));
}

bool ConfigFileChecker::isReadable(std::string const path, std::string const index)
{
	if (getPathType(index) == 1 && checkFile(index, 4) == 0)
		return true;
	if (getPathType(path + index) == 1 && checkFile(path + index, 4) == 0)
		return true;
	return false;
}

std::string ConfigFileChecker::readFile(std::string path)
{
	if (path.empty() || path.length() == 0)
		return NULL;
	std::ifstream configFile(path.c_str());
	if (!configFile || !configFile.is_open())
		return NULL;
	std::stringstream streamBind;
	streamBind << configFile.rdbuf();
	return (streamBind.str());
}

std::string ConfigFileChecker::getPath()
{
	return this->path;
}

int ConfigFileChecker::getSize()
{
	return this->size;
}
