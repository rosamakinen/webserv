#pragma once

#include "WebServer.hpp"
#include "Exceptions.hpp"
#include "HttpRequest.hpp"
#include "HttpResponse.hpp"
#include "FileHandler.hpp"
#include "ExceptionManager.hpp"
#include "Client.hpp"


class HttpRequestHandler
{
	private:
		bool 	findCgi(std::string uri);
		bool	validateCgi(std::string uri);
		
	public:
		HttpRequestHandler();
		~HttpRequestHandler();

		void	handleRequest(Client *client);
};
