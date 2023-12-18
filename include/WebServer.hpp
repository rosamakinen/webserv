#pragma once

#define CONNECTION_TIMEOUT	180000
#define MESSAGE_BUFFER		1048576

#include <vector>
#include <map>


#include <fstream>
#include <sstream>
#include <iostream>
#include <string>

#include <arpa/inet.h>

#include <sys/stat.h>	//fileops
#include <sys/file.h>	//fileops
#include <unistd.h>		//needed for checking file permissions
#include <time.h>

struct httpStatus
{
	unsigned int	code;
	std::string		message;
};

class Timer
{
	public:
		static const std::string GetTimeDate();
};

typedef std::map<std::string, std::string> simpleMap;
typedef std::map<std::string, std::vector<std::string> > vectorMap;
typedef std::map<std::string, vectorMap> locationMap;
