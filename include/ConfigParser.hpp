#pragma once

#include "../include/WebServer.hpp"

class ConfigParser
{
	private:
		std::ifstream				configFile;
		size_t						lineCount;
		size_t						start;
		nestedMap					nestedStrings;
		std::string					stringFile;
		std::string					execPath;
		std::vector<iterPair>		pairArray;
		std::vector<nestedMap>		configFileInfo;
		std::vector<std::string>	parsed;
		std::vector<size_t>			ports;
		std::vector<numberMap>		errors;

		bool						validateDirectory(char **environ, char **av);
		bool						checkPath(char **environ, char **av);
		bool						readFile();

	public:
		ConfigParser();
		~ConfigParser();

		// ERRORS
		class	ErrorEnviron : public std::exception
		{
			virtual const char	*what() const throw();
		};

};
