
#pragma once

#include "HttpRequest.hpp"
#include "FileHandler.hpp"
#include <string>
#include <iostream>
#include <sstream>
#include <map>

class HttpRequestParser
{
	private:
		void	parseParameters(std::string uri, std::map<std::string, std::string>& parameters);
		void	parseRequestLine(std::string &requestLine, Util::METHOD &method, std::string &uri, std::map<std::string, std::string>& parameters, std::string &version);
		HttpRequest::METHOD		parseMethod(std::string &requestLine);
		const std::string		parseVersion(std::string &requestLine);
		const std::string		parseMethodStr(std::string &requestLine);
		const std::string		parseUri(std::string &requestLine, std::map<std::string, std::string>& parameters);
		const std::string		getHeaderValue(std::map<std::string, std::string> &headers, std::string toFind);
		void	parseHeader(const std::string &request, std::map<std::string, std::string> &headers);
		void	findBody(std::string newLine, bool &bodyFound);
		void	parseBody(std::string newLine, std::string &body);
		int		compareAndSubstring(std::string method, std::string &requestLine);
		void	parseCgiMethod(Util::METHOD &method, std::string &uri);
		bool 	findCgi(std::string uri);
		bool	validateCgi(std::string uri);
		void	validateMethod(std::string& uri, Util::METHOD method, Server *server);

	public:
		HttpRequestParser();
		~HttpRequestParser();

		HttpRequest	*parseHttpRequest(std::string request, Server *server);
};
