#pragma once

#include "../include/WebServer.hpp"

class ConfigParser
{
	private:
		 void processLine(const std::string& line);

	public:
		static void parseConfig(const std::string& filename);
};
