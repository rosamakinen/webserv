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
		{
			//these clauses are added here so we wouldnt segfault untill we have the proper actions
			std::cout << "we would do post here" << std::endl;
			HttpResponse *response = new HttpResponse(std::pair<unsigned int, std::string>(200, "OK"), client->getRequest()->getUri());
			client->setResponse(response);
			client->setStatus(Client::STATUS::OUTGOING);
			return;
		}

		case HttpRequest::METHOD::DELETE:
		{
			//these clauses are added here so we wouldnt segfault untill we have the proper actions
			std::cout << "we would do delete here" << std::endl;
			HttpResponse *response = new HttpResponse(std::pair<unsigned int, std::string>(200, "OK"), client->getRequest()->getUri());
			client->setResponse(response);
			client->setStatus(Client::STATUS::OUTGOING);
			return;
		}

		case HttpRequest::METHOD::CGI_GET:
		{
			//these clauses are added here so we wouldnt segfault untill we have the proper actions
			std::cout << "we would execute get request cgi here" << std::endl;
			CgiHandler::executeCgi(*client->getRequest());
			HttpResponse *response = new HttpResponse(std::pair<unsigned int, std::string>(200, "OK"), client->getRequest()->getUri());
			client->setResponse(response);
			client->setStatus(Client::STATUS::OUTGOING);
			return;
		}

		case HttpRequest::METHOD::CGI_POST:
		{
			//these clauses are added here so we wouldnt segfault untill we have the proper actions
			std::cout << "we would execute post request cgi here" << std::endl;
			HttpResponse *response = new HttpResponse(std::pair<unsigned int, std::string>(200, "OK"), client->getRequest()->getUri());
			client->setResponse(response);
			client->setStatus(Client::STATUS::OUTGOING);
			return;
		}

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
