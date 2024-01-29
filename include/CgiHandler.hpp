
#pragma once

#include "WebServer.hpp"
#include "HttpRequest.hpp"

class CgiHandler
{
	private:
		//std::map<std::string, std::string> initCgiEnvironment(HttpRequest request);
		//std::map<std::string, std::string> initFromGetenv();


	public:
		static int	executeCgi(HttpRequest request);

};
