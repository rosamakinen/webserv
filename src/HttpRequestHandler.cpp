#include "../include/HttpRequestHandler.hpp"

void HttpRequestHandler::handleRequest(Client *client, Server *server)
{
	switch (client->getRequest()->getMethod())
	{
		case Util::METHOD::GET:
		{
			HttpResponse *response = new HttpResponse(std::pair<unsigned int, std::string>(200, "OK"), client->getRequest(), server);
			client->setResponse(response);
			client->setStatus(Client::STATUS::OUTGOING);
			return;
		}

		case Util::METHOD::POST:
		{
			//these clauses are added here so we wouldnt segfault untill we have the proper actions
			std::cout << "we would do post here" << std::endl;
			HttpResponse *response = new HttpResponse(std::pair<unsigned int, std::string>(200, "OK"), client->getRequest(), server);
			client->setResponse(response);
			client->setStatus(Client::STATUS::OUTGOING);
			return;
		}

		case Util::METHOD::DELETE:
		{
			//these clauses are added here so we wouldnt segfault untill we have the proper actions
			std::cout << "we would do delete here" << std::endl;
			HttpResponse *response = new HttpResponse(std::pair<unsigned int, std::string>(200, "OK"), client->getRequest(), server);
			client->setResponse(response);
			client->setStatus(Client::STATUS::OUTGOING);
			return;
		}

		case Util::METHOD::CGI_GET:
		{
			//these clauses are added here so we wouldnt segfault untill we have the proper actions
			std::cout << "we would execute get request cgi here" << std::endl;
			std::string cgiResponse = CgiHandler::executeCgi(*client->getRequest());
			HttpResponse *response = new HttpResponse(std::pair<unsigned int, std::string>(200, "OK"), client->getRequest(), server);
			response->setCgiResponse(cgiResponse);
			client->setResponse(response);
			client->setStatus(Client::STATUS::OUTGOING);
			return;
		}

		case Util::METHOD::CGI_POST:
		{
			//these clauses are added here so we wouldnt segfault untill we have the proper actions
			std::cout << "we would execute post request cgi here" << std::endl;
			std::string cgiResponse = CgiHandler::executeCgi(*client->getRequest());
			HttpResponse *response = new HttpResponse(std::pair<unsigned int, std::string>(200, "OK"), client->getRequest(), server);
			response->setCgiResponse(cgiResponse);
			client->setResponse(response);
			client->setStatus(Client::STATUS::OUTGOING);
			return;
		}

		default :
			throw MethodNotAllowedException("Method not allowed");
			break;
	}

	HttpResponse *response = new HttpResponse(ExceptionManager::getErrorStatus(InternalException("Something went wrong")), client->getRequest(), server);
	client->setResponse(response);
	client->setStatus(Client::STATUS::OUTGOING);
}

HttpRequestHandler::HttpRequestHandler()
{
}

HttpRequestHandler::~HttpRequestHandler()
{
}
