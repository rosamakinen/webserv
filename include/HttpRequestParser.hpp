
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
		// std::string									parseHost(std::map<std::string, std::string> &headers, std::string &host);
		std::string									getHeaderValue(std::map<std::string, std::string> &headers, std::string toFind);
		const std::map<std::string, std::string>	parseHeaders(const std::string &request, std::map<std::string, std::string> &headers);
		bool										findBody(std::string newLine, bool &bodyFound);
		const std::string							parseBody(std::string newLine, std::string &body);
		int											compareMethod(std::string method, std::string &requestLine);
};
