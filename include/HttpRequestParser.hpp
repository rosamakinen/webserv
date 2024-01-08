
#pragma once

#include "HttpRequest.hpp"
#include <string>
#include <iostream>
#include <sstream>
#include <map>

class HttpRequestParser
{
	public:
		HttpRequestParser();
		~HttpRequestParser();

		HttpRequest									parseHttpRequest(std::string request);
		void										parseRequestLine(std::string &requestLine, std::string &method, std::string &uri, std::string &version);
		std::string									parseMethod(std::string &requestLine);
		const std::string							parseVersion(std::string &requestLine);
		const std::string							parseUri(std::string &requestLine);
		std::string									parseHost(std::map<std::string, std::string> &headers);
		const std::map<std::string, std::string>	parseHeaders(const std::string &request, std::map<std::string, std::string> &headers);
		const std::string							parseBody();
		int											compareMethod(std::string method, std::string &requestLine);
};
