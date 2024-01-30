
#pragma once

#include <iostream>
#include <cstdlib>

#include "HttpResponse.hpp"
#include "Util.hpp"
#include "Server.hpp"

class HttpResponseParser
{
	public:
		static std::string Parse(const HttpResponse& response);
};
