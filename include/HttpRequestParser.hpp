
#pragma once

#include "HttpRequest.hpp"
#include <string>
#include <iostream>
#include <sstream>

class HttpRequestParser
{
	private:
		std::string	_requestLine;

		std::string	_tempMethod;
		std::string	_tempVersion;
		std::string	_tempUri;
		std::string	_tempHost;
		std::string	_tempBody;
		int			_tempContentLength;


	public:
		HttpRequestParser();
		~HttpRequestParser();
		HttpRequestParser(const HttpRequestParser& rhs);

		HttpRequestParser&	operator=(const HttpRequestParser& rhs);

		HttpRequest&				parseHttpRequest(std::string request);
		void						parseRequestLine(std::string newLine);
		void						parseRequestLineContent();
		std::string					parseMethod(std::string requestLine);
		const std::string			parseVersion(std::string requestLine);
		const std::string			parseUri(std::string requestLine);
		const std::string			parseHost();
		const std::string			parseBody();
		const int					parseContentLength();
		int							compareMethod(int lastIndex, std::string method, std::string requestLine);
};
