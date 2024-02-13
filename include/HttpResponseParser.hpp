#ifndef HTTPRESPONSEPARSER_HPP
#define HTTPRESPONSEPARSER_HPP

#include "WebServer.hpp"
#include "HttpResponse.hpp"
#include "Util.hpp"
#include "Server.hpp"

class HttpResponseParser
{
	public:
		static std::string Parse(const HttpResponse& response);
};

#endif
