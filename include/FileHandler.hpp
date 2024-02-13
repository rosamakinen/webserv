#ifndef FILEHANDLER_HPP
#define FILEHANDLER_HPP

#include "HttpRequest.hpp"
#include "WebServer.hpp"
#include "Util.hpp"
#include "Server.hpp"

class FileHandler
{
	public:
		static std::string getFileContent(std::string filename);
		static std::string getErrorFileContent(unsigned int status, Server *server);
		static std::string getFileResource(HttpRequest *request, std::ios_base::openmode mode);
		static std::string getFilePath(std::string relativePath);
		static std::string buildDirListing(std::string full_path);
};

#endif
