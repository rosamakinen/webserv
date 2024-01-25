
#pragma once

#include <iostream>
#include <fstream>

class FileHandler
{
	public:
		static std::string getFileContent(std::string filename);
		static std::string getErrorFileContent(unsigned int status);
		static std::string getFileResource(std::string path, std::ios_base::openmode mode);
		static std::string getFilePath(std::string relativePath);
		static std::string readBinary(std::ifstream &file);
};
