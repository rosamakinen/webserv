#include "HttpRequestHandler.hpp"
#include "FileHandler.hpp"
#include "ExceptionManager.hpp"

HttpResponse HttpRequestHandler::handleRequest(HttpRequest input)
{
	switch (input.getMethod())
	{
		case HttpRequest::METHOD::GET:
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

HttpRequestHandler::HttpRequestHandler()
{

}

HttpRequestHandler::~HttpRequestHandler()
{
}
