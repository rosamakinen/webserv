#include "HttpRequestParser.hpp"

HttpRequestParser::HttpRequestParser()
{
}

HttpRequestParser::~HttpRequestParser()
{
}

HttpRequest HttpRequestParser::parseHttpRequest(std::string requestInput)
{
	std::stringstream					ss(requestInput);
	std::string							requestLine = "";
	std::string							newLine = "";
	std::string							method = "";
	std::string							uri = "";
	std::string							version = "";
	std::string							body = "body";
	std::string							host = "host";
	std::map<std::string, std::string>	headers;

	while (getline(ss, newLine, '\n'))
	{
		if (requestLine.empty())
		{
			requestLine = newLine;
			std::cout << "newRequestLine at start: " << requestLine << std::endl;
			parseRequestLine(requestLine, method, uri, version);
		}
		parseHeaders(newLine, headers);
	}
	host = parseHost(headers);
	std::cout << "headers map is: " << std::endl;
	for (const auto &pair : headers)
	{
		std::cout << pair.first << " ***and*** " << pair.second << std::endl;
	}
	std::cout << "*****host is: " << host << std::endl;
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

std::string HttpRequestParser::parseHost(std::map<std::string, std::string> &headers)
{
	std::map<std::string, std::string>::iterator it;
	std::string host = "";

	it = headers.find("Host");
	if (it == headers.end())
		throw BadRequestException("Host not found");
	else
		host = it->second;
	std::cout << "parse host has parsed: " << host << std::endl;
	return host;
}

const std::map<std::string, std::string> HttpRequestParser::parseHeaders(const std::string &line, std::map<std::string, std::string> &headers)
{
	size_t pos = line.find(':');
	if (pos != std::string::npos)
	{
		std::string key = line.substr(0, pos);
		std::string value = line.substr(pos + 1);

		// // Trim leading and trailing whitespace
		// key.erase(key.begin(), std::find_if(key.begin(), key.end(), [](unsigned char ch) { return !std::isspace(ch); }));
		// key.erase(std::find_if(key.rbegin(), key.rend(), [](unsigned char ch) { return !std::isspace(ch); }).base(), key.end());

		// value.erase(value.begin(), std::find_if(value.begin(), value.end(), [](unsigned char ch) { return !std::isspace(ch); }));
		// value.erase(std::find_if(value.rbegin(), value.rend(), [](unsigned char ch) { return !std::isspace(ch); }).base(), value.end());

		if (headers[key].empty())
			headers[key] = value;
	}
	return headers;
}

const std::string HttpRequestParser::parseBody()
{
	return std::string();
}
