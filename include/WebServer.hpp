#pragma once

#define CONNECTION_TIMEOUT	60
#define MESSAGE_BUFFER		1048576

#include <vector>
#include <map>
#include <utility> // pair

#include <fstream>
#include <sstream>
#include <iostream>
#include <string>

#include <arpa/inet.h>

#include <sys/stat.h>	//fileops
#include <sys/file.h>	//fileops
#include <unistd.h>		//needed for checking file permissions

typedef std::map<std::string, std::string> simpleMap;
typedef std::map<std::string, std::vector<std::string> > vectorMap;
typedef std::map<std::string, vectorMap> locationMap;
