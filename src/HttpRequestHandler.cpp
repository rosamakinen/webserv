#include "../include/HttpRequestHandler.hpp"

bool HttpRequestHandler::redirectClient(Client *client)
{
	if (client->getRequest()->getIsDirListing())
		return true;
	return false;
}

void HttpRequestHandler::parseRedirResponse(Client *client, Server *server)
{
	HttpResponse *response = new HttpResponse();
	try
	{
		response->setStatus(std::pair<unsigned int, std::string>(301, "Moved Permanently"));
	}
	catch(const Exception& e)
	{
		delete response;
		response = parseErrorResponse(server, ExceptionManager::getErrorStatus(e));
	}

	client->setResponse(response);
}

void HttpRequestHandler::parseOkResponse(Client *client, Server *server)
{
	HttpResponse *response = new HttpResponse();
	try
	{
		if (client->getRequest()->getMethod() == Util::METHOD::POST)
			response->setStatus(std::pair<unsigned int, std::string>(201, "Created"));
		else if (client->getRequest()->getMethod() == Util::METHOD::DELETE)
			response->setStatus(std::pair<unsigned int, std::string>(204, "No Content"));
		else
			response->setStatus(std::pair<unsigned int, std::string>(200, "OK"));
		response->setResponseBody(client->getRequest(), server);
	}
	catch(const Exception& e)
	{
		delete response;
		response = parseErrorResponse(server, ExceptionManager::getErrorStatus(e));
	}

	client->setResponse(response);
}

HttpResponse *HttpRequestHandler::parseErrorResponse(Server *server, std::pair<unsigned int, std::string> status)
{
	HttpResponse *response = new HttpResponse();
	try
	{
		response->setStatus(status);
		response->setResponseBody(nullptr, server);
	}
	catch(const Exception& e)
	{
		delete response;
		throw;
	}

	return response;
}

void HttpRequestHandler::handleRequest(Client *client, Server *server)
{
	try
	{
		if (redirectClient(client))
		{
			parseRedirResponse(client, server);
			return ;
		}

		std::cout << "hello with " << client->getRequest()->getMethod() << std::endl;
		switch (client->getRequest()->getMethod())
		{
			case Util::METHOD::GET:
			{
				parseOkResponse(client, server);
				break;
			}

			case Util::METHOD::POST:
			{
				Methods::executePost(client->getRequest(), client->getServer());
				parseOkResponse(client, server);
				break;
			}

			case Util::METHOD::DELETE:
			{
				Methods::executeDelete(*client->getRequest());
				parseOkResponse(client, server);
				break;
			}

			case Util::METHOD::CGI_GET:
			{
				std::string cgiResponse = CgiHandler::executeCgi(*client->getRequest(), server);
				parseOkResponse(client, server);
				client->getResponse()->setCgiResponse(cgiResponse);
				break;
			}

			case Util::METHOD::CGI_POST:
			{
				std::string cgiResponse = CgiHandler::executeCgi(*client->getRequest(), server);
				parseOkResponse(client, server);
				client->getResponse()->setCgiResponse(cgiResponse);
				break;
			}

			default :
				throw NotImplementedException("Method not allowed");
				break;
		}
	}
	catch (const Exception& e)
	{
		client->setResponse(parseErrorResponse(server, ExceptionManager::getErrorStatus(e)));
	}

}

HttpRequestHandler::HttpRequestHandler()
{
}

HttpRequestHandler::~HttpRequestHandler()
{
}
