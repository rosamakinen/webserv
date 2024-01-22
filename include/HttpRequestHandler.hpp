#pragma once

#include "WebServer.hpp"
#include "Exceptions.hpp"
#include "HttpRequest.hpp"
#include "HttpResponse.hpp"
#include "ExceptionManager.hpp"
#include "FileHandler.hpp"

class HttpRequestHandler
{
	private:
		bool 	findCgi(std::string uri);
		bool	validateCgi(std::string uri);
		
	public:
		HttpRequestHandler();
		~HttpRequestHandler();
		HttpResponse	handleRequest(HttpRequest input);
};
