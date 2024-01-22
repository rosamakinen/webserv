#include "HttpRequestHandler.hpp"

void HttpRequestHandler::handleRequest(Client *client)
{
	switch (client->getRequest()->getMethod())
	{
		case HttpRequest::METHOD::GET:
		{
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

HttpRequestHandler::HttpRequestHandler()
{
}

HttpRequestHandler::~HttpRequestHandler()
{
}
