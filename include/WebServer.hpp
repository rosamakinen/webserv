#pragma once

#define CONNECTION_TIMEOUT	180000
#define MESSAGE_BUFFER		1048576
#define CGI_TIMEOUT_LIMIT	10

#define HTTP_VERSION		"HTTP/1.1"
#define GATEWAY_VERSION		"CGI/1.1"
#define HTTP_LINEBREAK		"\r\n"
#define SPACE				" "
#define WHITESPACE			" \t"
#define SHEBANG				"#!"

#define PARSEHOST			"host"
#define PARSELISTEN			"listen"
#define PARSENAME			"server_name"
#define PARSESIZE			"client_max_body_size"
#define SERVERBLOCK			"server"
#define MAINBLOCK			"main"
#define LOCATIONBLOCK		"location"
#define CGI_LOCATION		"cgi-bin"
#define AUTO_INDEX			"autoindex"
#define CONFIG_TRUE			"true"
#define LOCAL_DIR			"directory"
#define INDEX				"index"
#define ERRORPAGE_LOCATION	"error_page"

#define DEFAULT_ERRORPAGES_PATH "/public_www/error_pages/"

#define HTTP_METHOD			"method"
#define HTTP_GET			"GET"
#define HTTP_POST			"POST"
#define HTTP_DELETE			"DELETE"
#define HTTP_NONE			"NONE"

#define EXT_HTML			".html"
#define EXT_CSS				".css"
#define EXT_JPEG			".jpeg"

#include "../include/Exceptions.hpp"

#include <vector>
#include <map>
#include <set>
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
