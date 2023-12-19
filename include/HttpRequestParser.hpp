
#pragma once

#include "HttpRequest.hpp"
#include <string>
#include <iostream>
#include <sstream>

class HttpRequestParser
{
	public:
		HttpRequestParser();
		~HttpRequestParser();

		HttpRequest					parseHttpRequest(std::string request);
		void						parseRequestLine(std::string &requestLine, std::string &method, std::string &uri, std::string &version);
		std::string					parseMethod(std::string &requestLine);
		const std::string			parseVersion(std::string &requestLine);
		const std::string			parseUri(std::string &requestLine);
		const std::string			parseHost();
		const std::string			parseBody();
		int							parseContentLength();
		int							compareMethod(std::string method, std::string &requestLine);
};
