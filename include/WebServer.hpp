#ifndef WEBSERVER_HPP
#define WEBSERVER_HPP

#define VERSION "1.0"

#define CONNECTION_TIMEOUT	2000
#define MESSAGE_BUFFER		1048576
#define CGI_TIMEOUT_LIMIT	5
#define CLIENT_TIMEOUT		CGI_TIMEOUT_LIMIT + 5

#define WHITELIST	"localhost"
#define LOCAL_IP	"127.0.0.1"

#define HTTP_VERSION		"HTTP/1.1"
#define GATEWAY_VERSION		"CGI/1.1"
#define HTTP_LINEBREAK		"\r\n"
#define SPACE				" "
#define WHITESPACE			" \t"
#define SHEBANG				"#!"
#define BACKSLASH			'/'
#define MLTP_LINEBREAK		"--"

#define H_CONTENT_LENGTH	"Content-Length"
#define H_CONTENT_TYPE		"Content-Type"
#define H_ENCODING			"Transfer-Encoding"

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
#define UPLOAD_DIR			"upload"
#define INDEX				"index"
#define ERRORPAGE_LOCATION	"error_page"
#define SERVER_DEFAULT_NAME	"default"
#define TRUE				"true"
#define DEFAULT_FILE		"default.txt"
#define REDIR				"return"

#define DEFAULT_RESPONSEPAGES_PATH "/public_www/response_pages/"
#define REDIRECT_PATH "/public_www/response_pages/301"

#define HTTP_METHOD			"method"
#define HTTP_GET			"GET"
#define HTTP_POST			"POST"
#define HTTP_DELETE			"DELETE"
#define HTTP_NONE			"NONE"
#define HTTP_PUT			"PUT"
#define HTTP_HEAD			"HEAD"
#define HTTP_OPTIONS		"OPTIONS"
#define HTTP_CONNECT		"CONNECT"
#define HTTP_TRACE			"TRACE"
#define HTTP_PATCH			"PATCH"

#define EXT_HTML			".html"
#define EXT_CSS				".css"
#define EXT_JPEG			".jpeg"

#define CT_TXT	"text/plain"
#define CT_TXT2	"plain/text"
#define CT_MLTP "multipart/form-data"
#define CT_FRM	"application/x-www-form-urlencoded"

#include <vector>
#include <map>
#include <set>
#include <utility> // pair

#include <fstream>
#include <sstream>
#include <iostream>
#include <string>

#include <sys/socket.h> // For socket functions
#include <sys/stat.h>	//fileops
#include <dirent.h>
#include <unistd.h>		//needed for checking file permissions
#include <stdlib.h>
#include <time.h>
#include <limits.h>
#include <sys/poll.h>
#include <netinet/in.h> // For sockaddr_in
#include <poll.h>
#include <fcntl.h>

#include <stdexcept> // invalid_argument
#include <cstdlib> // For exit() and EXIT_FAILURE
#include <cerrno>

typedef std::map<std::string, std::vector<std::string> > vectorMap;
typedef std::map<std::string, vectorMap> locationMap;

#endif
