#pragma once

#include "WebServer.hpp"
#include "Exceptions.hpp"
#include "HttpRequest.hpp"
#include "HttpResponse.hpp"


class HttpRequestHandler
{
	public:
		HttpRequestHandler();
		~HttpRequestHandler();
		HttpResponse	handleRequest(HttpRequest input);
};
