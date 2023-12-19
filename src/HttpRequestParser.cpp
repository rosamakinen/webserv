#include "HttpRequestParser.hpp"

HttpRequestParser::HttpRequestParser()
{
}

HttpRequestParser::~HttpRequestParser()
{
}

HttpRequest HttpRequestParser::parseHttpRequest(std::string requestInput)
{
	std::stringstream	ss(requestInput);
	std::string			requestLine = "";
	std::string			newLine = "";
	std::string			method = "";
	std::string			uri = "";
	std::string			version = "";
	std::string			body = "body";
	std::string			host = "host";

	while (getline(ss, newLine, '\n'))
	{
		if (requestLine.empty())
		{
			requestLine = newLine;
			std::cout << "newRequestLine at start: " << requestLine << std::endl;
			parseRequestLine(requestLine, method, uri, version);
		}
		//TODO: headers and body parsing
		// if (headers.empty())
			//	parseHeaders(newLine);
		// if (body.empty())
			//	parseBody(newline);
	}
	HttpRequest request(method, version, uri, host, body, body.length());
	return request;
}

void HttpRequestParser::parseRequestLine(std::string &requestLine, std::string &method, std::string &uri, std::string &version)
{
	std::cout << "parsing reqline " << requestLine << std::endl;
	if (requestLine.empty())
	{
		throw BadRequestException("Empty requestline");
	}
	if (method.empty())
		method = parseMethod(requestLine);
	if (uri.empty())
		uri = parseUri(requestLine);
	if (version.empty())
		version = parseVersion(requestLine);
}

std::string HttpRequestParser::parseMethod(std::string &requestLine)
{
	std::string method;
	if (compareMethod("GET ", requestLine) == 0)
			method = GET;
	else if (compareMethod("POST ", requestLine) == 0)
			method = POST;
	else if (compareMethod("DELETE ", requestLine) == 0)
			method = DELETE;
	else
		throw BadRequestException("Wrong method type");
	return method;
}

int	HttpRequestParser::compareMethod(std::string method, std::string &requestLine)
{
	if (requestLine.compare(0, method.length(), method) == 0)
	{
		requestLine = requestLine.substr(method.length(), requestLine.length());
		return 0;
	}
	return 1;
}

const std::string HttpRequestParser::parseVersion(std::string &requestLine)
{
	std::string version = "HTTP/1.1";
	if (requestLine.compare(HTTP_VERSION) == 0)
		throw BadRequestException("Wrong Http version");
	return version;
}

const std::string HttpRequestParser::parseUri(std::string &requestLine)
{
	std::string uri;
	size_t		pos;
	pos = requestLine.find(' ');
	uri = requestLine.substr(0, pos);
	requestLine = requestLine.substr(pos, requestLine.length());
	return uri;
}

const std::string HttpRequestParser::parseHost()
{
	return std::string();
}

const std::string HttpRequestParser::parseBody()
{
	return std::string();
}

int HttpRequestParser::parseContentLength()
{
	return 1;
}
