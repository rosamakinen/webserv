
#pragma once

#include <iostream>
#include <fstream>

class FileHandler
{
	public:
		static std::string getFileContent(std::string filename);
		static std::string getErrorFileContent(unsigned int status);
		static std::string getFileResource(std::string path);
		static std::string getFilePath(std::string relativePath);
};
