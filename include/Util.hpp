#ifndef UTIL_HPP
#define UTIL_HPP

#include "WebServer.hpp"
#include "Exceptions.hpp"

class Util
{
	public:
		enum METHOD
		{
			NONE,
			GET,
			CGI_GET,
			POST,
			CGI_POST,
			DELETE
		};

		static const std::string	getTimeDateString();
		static const std::string	translateMethod(Util::METHOD method);
		static std::string getDirectoryFromUri(const std::string &uri);
		static std::string getFileFromUri(const std::string &uri);
		static void freeStringArray(char **string);
};

#endif
