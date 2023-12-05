
#pragma once

#include "HttpRequest.hpp"
#include <string>

class HttpRequestParser
{
	private:
		std::string	method;
		std::string	version;
		std::string	uri;
		std::string	host;
		std::string	body;
		int			contentLength;

		std::string	requestLine;

	public:
		HttpRequestParser();
		~HttpRequestParser();
		HttpRequestParser(const HttpRequestParser& rhs);

		HttpRequestParser&	operator=(const HttpRequestParser& rhs);

		HttpRequest&				parseHttpRequest(std::string request);
		void						parseRequestLine(std::string requestLine);
		const std::string			parseMethod();
		const std::string			parseVersion();
		const std::string			parseUri();
		const std::string			parseHost();
		const std::string			parseBody();
		const int					parseContentLength();
};
