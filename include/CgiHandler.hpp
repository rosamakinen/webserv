#ifndef CGIHANDLER_HPP
#define CGIHANDLER_HPP

#include "WebServer.hpp"
#include "HttpRequest.hpp"
#include "FileHandler.hpp"
#include "Util.hpp"

class CgiHandler
{
	public:
		static std::string	executeCgi(HttpRequest request);
};

#endif
