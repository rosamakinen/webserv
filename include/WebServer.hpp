#pragma once

#define CONNECTION_TIMEOUT	60
#define MESSAGE_BUFFER		1048576

#include <vector>
#include <map>

#include <fstream>
#include <sstream>
#include <iostream>
#include <string>

#include <sys/stat.h>	//fileops
#include <sys/file.h>	//fileops
#include <unistd.h>		//needed for checking file permissions

typedef std::vector<std::string>::iterator iter;
typedef std::map<std::string, std::map<std::string, std::string>> nestedMap;
typedef std::map<std::string, std::string> stringMap;
typedef std::map<ssize_t, std::string> numberMap;
typedef std::pair<iter, iter> iterPair;
