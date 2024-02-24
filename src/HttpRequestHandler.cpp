#include "../include/HttpRequestHandler.hpp"

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
		switch (client->getRequest()->getMethod())
		{
			case Util::METHOD::GET:
			{
				parseOkResponse(client, server);
				return;
			}

			case Util::METHOD::POST:
			{
				Methods::executePost(client->getRequest(), client->getServer());
				parseOkResponse(client, server);
				return ;
			}

			case Util::METHOD::DELETE:
			{
				Methods::executeDelete(*client->getRequest());
				parseOkResponse(client, server);
				return;
			}

			case Util::METHOD::CGI_GET:
			{
				std::string cgiResponse = CgiHandler::executeCgi(*client->getRequest(), server);
				parseOkResponse(client, server);
				client->getResponse()->setCgiResponse(cgiResponse);
				return;
			}

			case Util::METHOD::CGI_POST:
			{
				std::string cgiResponse = CgiHandler::executeCgi(*client->getRequest(), server);
				parseOkResponse(client, server);
				client->getResponse()->setCgiResponse(cgiResponse);
				return;
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
