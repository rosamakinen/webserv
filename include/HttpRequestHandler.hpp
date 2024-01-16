#pragma once

#include "WebServer.hpp"
#include "Exceptions.hpp"
#include "HttpRequest.hpp"


class HttpRequestHandler
{
	private:
		void	HandleRequest(HttpRequest input);

	public:
		HttpRequestHandler(HttpRequest input);
		~HttpRequestHandler();
};
