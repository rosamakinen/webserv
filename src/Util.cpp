#include "../include/WebServer.hpp"
#include "../include/Util.hpp"
#include "Util.hpp"

const std::string Util::getTimeDateString()
{
	// Initialize timezone information
	tzset();
	// Get local machine time and convert to UTC time
	time_t now = time(0);
	struct tm tm = *gmtime(&now);

	// Stringify the struct to a string representation
	char date[1000];
	strftime(date, sizeof(date), "%a, %d %b %Y %T %Z", &tm);
	return (std::string(date));
}

const std::string Util::translateMethod(Util::METHOD method)
{
	switch (method)
	{
		case Util::METHOD::GET :
			return HTTP_GET;
		break;

		case Util::METHOD::POST :
			return HTTP_POST;
		break;

		case Util::METHOD::DELETE :
			return HTTP_DELETE;
		break;

		case Util::METHOD::CGI_GET :
			return HTTP_GET;
		break;

		case Util::METHOD::CGI_POST :
			return HTTP_POST;
		break;

		case Util::METHOD::NONE :
			return HTTP_NONE;

		default:
		break;
	}

	return nullptr;
}

std::string Util::getDirectoryFromUri(const std::string& uri)
{
	size_t pos = uri.find_last_of('/');
	return (pos == std::string::npos) ? "" : uri.substr(0, pos + 1);
}

std::string Util::getFileFromUri(const std::string& uri)
{
	size_t pos = uri.find_last_of('/');
	return (pos == std::string::npos) ? uri : uri.substr(pos + 1);
}

void Util::freeStringArray(char **string)
{
	for (int i = 0; string[i]; i++)
		delete [] string[i];
	delete [] string;
}