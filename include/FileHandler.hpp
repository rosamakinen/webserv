
#pragma once

#include <iostream>
#include <fstream>

class FileHandler
{
	public:
		static std::string getFileContent(std::string filename, bool addCrLf);
};
