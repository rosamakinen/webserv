#include "HttpRequestHandler.hpp"
#include "FileHandler.hpp"

HttpResponse HttpRequestHandler::handleRequest(HttpRequest input)
{
	bool cgiFound = false;
	cgiFound = findCgi(input.getUri());
	switch (input.getMethod())
	{
		case HttpRequest::METHOD::GET:
			if (cgiFound == true)
				std::cout << "here we would go to execute the script" << std::endl;
			return HttpResponse(std::pair<unsigned int, std::string>(200, "OK"), input.getUri());
			break;

		case HttpRequest::METHOD::POST:

			break;

		case HttpRequest::METHOD::DELETE:

			break;

		default :
			throw MethodNotAllowedException("Method not allowed");
			break;
	}
	return HttpResponse(ExceptionManager::getErrorStatus(InternalException("Something went wrong")), "");
}

bool HttpRequestHandler::findCgi(std::string uri)
{
	size_t found = uri.find("/cgi_bin");
	if (found != std::string::npos)
	{
		bool ret = validateCgi(uri);
		return ret;
	}
	return false;
}

bool HttpRequestHandler::validateCgi(std::string uri)
{
	std::string suffix = ".py";
	std::string fullPath = FileHandler::getFilePath(uri);

	if (access(fullPath.c_str(), F_OK) == 0)
	{
		int pos = fullPath.find(suffix);
		std::string extension = fullPath.substr(pos, fullPath.length());
		if (extension.compare(suffix) == 0)
			return true;
	}
	throw BadRequestException("Bad CGI request");
	return false;
}

HttpRequestHandler::HttpRequestHandler()
{

}

HttpRequestHandler::~HttpRequestHandler()
{
}
