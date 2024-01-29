#pragma once

#include "WebServer.hpp"
#include "Exceptions.hpp"
#include "HttpRequest.hpp"
#include "HttpResponse.hpp"
#include "FileHandler.hpp"
#include "ExceptionManager.hpp"
#include "Client.hpp"
#include "CgiHandler.hpp"

class CgiHandler;

class HttpRequestHandler
{
	public:
		HttpRequestHandler();
		~HttpRequestHandler();

		void	handleRequest(Client *client);
};
