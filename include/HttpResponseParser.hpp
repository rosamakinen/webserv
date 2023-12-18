
#pragma once

#include <iostream>

#include "HttpResponse.hpp"
#include "Server.hpp"

class HttpResponseParser
{
	public:
		static std::string Parse(const HttpResponse& response, Server *server);
};
