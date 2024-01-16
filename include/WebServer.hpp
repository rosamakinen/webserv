#pragma once

#define CONNECTION_TIMEOUT	180000
#define MESSAGE_BUFFER		1048576

#define HTTP_VERSION		"HTTP/1.1"
#define HTTP_LINEBREAK		"\r\n"
#define SPACE				" "
#define HTTP_GET			"GET"
#define HTTP_POST			"POST"
#define HTTP_DELETE			"DELETE"

#include "../include/Exceptions.hpp"

#include <vector>
#include <map>
#include <utility> // pair
#include <algorithm>
#include <typeinfo>

#include <fstream>
#include <sstream>
#include <iostream>
#include <string>

#include <arpa/inet.h>

#include <sys/stat.h>	//fileops
#include <sys/file.h>	//fileops
#include <unistd.h>		//needed for checking file permissions
#include <stdlib.h>
#include <time.h>
#include <limits.h>

#include <stdexcept> // invalid_argument

typedef std::map<std::string, std::string> simpleMap;
typedef std::map<std::string, std::vector<std::string> > vectorMap;
typedef std::map<std::string, vectorMap> locationMap;
