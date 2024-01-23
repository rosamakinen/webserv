#include "HttpRequestHandler.hpp"
#include "FileHandler.hpp"

void HttpRequestHandler::handleRequest(Client *client)
{
	bool cgiFound = false;
	cgiFound = findCgi(client->getRequest()->getUri());

	switch (client->getRequest()->getMethod())
	{
		case HttpRequest::METHOD::GET:
		{
			if (cgiFound == true)
				std::cout << "here we would go to execute the script" << std::endl;
			HttpResponse *response = new HttpResponse(std::pair<unsigned int, std::string>(200, "OK"), client->getRequest()->getUri());
			client->setResponse(response);
			client->setStatus(Client::STATUS::OUTGOING);
			return;
		}

		case HttpRequest::METHOD::POST:
			return;

		case HttpRequest::METHOD::DELETE:
			return;

		default :
			throw MethodNotAllowedException("Method not allowed");
			break;
	}

	HttpResponse *response = new HttpResponse(ExceptionManager::getErrorStatus(InternalException("Something went wrong")), "");
	client->setResponse(response);
	client->setStatus(Client::STATUS::OUTGOING);
}

bool HttpRequestHandler::findCgi(std::string uri)
{
	size_t found = uri.find("/cgi-bin");
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
		size_t pos = fullPath.find(suffix);
		if (pos != std::string::npos)
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
