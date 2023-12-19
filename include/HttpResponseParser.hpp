
#pragma once

#include <iostream>
#include <cstdlib>

#include "HttpResponse.hpp"
#include "Server.hpp"
#include "ScalarConverter.hpp"

class HttpResponseParser
{
	public:
		static std::string Parse(const HttpResponse& response, Server *server);
};
