
#pragma once

#include "WebServer.hpp"
#include "HttpRequest.hpp"
#include "FileHandler.hpp"

class CgiHandler
{

	public:
		static int	executeCgi(HttpRequest request);

};
