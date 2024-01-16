
#pragma once

#include <iostream>
#include <fstream>

class FileHandler
{
	public:
		static std::string getFileContent(std::string filename, bool addCrLf);
		static std::string getErrorFileContent(unsigned int status);
};