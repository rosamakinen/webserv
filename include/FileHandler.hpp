
#pragma once

#include <iostream>
#include <fstream>
#include <sys/types.h>
#include <dirent.h>
#include "Server.hpp"

class FileHandler
{
	public:
		static std::string getFileContent(std::string filename);
		static std::string getErrorFileContent(unsigned int status);
		static std::string getFileResource(std::string path, std::ios_base::openmode mode, Server *server);
		static std::string getFilePath(std::string relativePath);
		static std::string buildDirListing(std::string full_path);
		static std::string readBinary(std::ifstream &file);
};
