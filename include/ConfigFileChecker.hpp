#pragma once

#include <sys/stat.h>
#include <sys/file.h>
#include <unistd.h>		//needed for checking file permissions
#include <fstream>
#include <sstream>
#include <iostream>

class ConfigFileChecker
{
	private:
		std::string	path;
		size_t		size;

	public:
		ConfigFileChecker();
		~ConfigFileChecker();
		ConfigFileChecker(std::string const path);

		static int	getPathType(std::string const path);
		static int	checkFile(std::string const path, int mode);
		static bool	isReadable(std::string const path, std::string const index);
		std::string	readFile(std::string path);

		std::string	getPath();
		int			getSize();
};
