
#pragma once

#include "HttpRequest.hpp"
#include <string>
#include <iostream>
#include <sstream>

class HttpRequestParser
{
	private:
		std::string	requestLine;

		std::string	method;
		std::string	version;
		std::string	uri;
		std::string	host;
		std::string	body;
		int			contentLength;


	public:
		HttpRequestParser();
		~HttpRequestParser();
		HttpRequestParser(const HttpRequestParser& rhs);

		HttpRequestParser&	operator=(const HttpRequestParser& rhs);

		HttpRequest&				parseHttpRequest(std::string request);
		void						parseRequestLine(std::string requestInput);
		const std::string			parseMethod(std::string requestLine);
		const std::string			parseVersion(std::string requestLine);
		const std::string			parseUri(std::string requestLine);
		const std::string			parseHost();
		const std::string			parseBody();
		const int					parseContentLength();
};
