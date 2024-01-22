#include "HttpRequestParser.hpp"

HttpRequestParser::HttpRequestParser() {}

HttpRequestParser::~HttpRequestParser() {}

HttpRequest *HttpRequestParser::parseHttpRequest(std::string requestInput)
{
	std::stringstream					ss(requestInput);
	std::string requestLine, uri, version;
	HttpRequest::METHOD method;
	getline(ss, requestLine);
	parseRequestLine(requestLine, method, uri, version);

	std::map<std::string, std::string>	headers;
	while (getline(ss, requestLine))
	{
		if (requestLine.compare("\r") == 0)
			break;
		parseHeader(requestLine, headers);
	}

	std::string	body = "";
	while (getline(ss, requestLine))
	{
		if (requestLine.compare("\r") == 0)
			break;
		std::cout << "Parsing line '" << requestLine << "'"<< std::endl;
		parseBody(requestLine, body);
	}

	std::string host = getHeaderValue(headers, "Host");
	HttpRequest *request = new HttpRequest(method, version, uri, host, "body", 14);
	return request;
}

void HttpRequestParser::parseRequestLine(std::string &requestLine, HttpRequest::METHOD& method, std::string &uri, std::string &version)
{
	if (requestLine.empty())
		throw BadRequestException("Empty requestline");

	method = parseMethod(requestLine);
	uri = parseUri(requestLine);
	version = parseVersion(requestLine);
}

HttpRequest::METHOD HttpRequestParser::parseMethod(std::string &requestLine)
{
	if (compareAndSubstring("GET ", requestLine) == 0)
		return HttpRequest::METHOD::GET;
	else if (compareAndSubstring("POST ", requestLine) == 0)
		return HttpRequest::METHOD::POST;
	else if (compareAndSubstring("DELETE ", requestLine) == 0)
		return HttpRequest::METHOD::DELETE;
	parseMethodStr(requestLine);
	return HttpRequest::METHOD::NONE;
}

int	HttpRequestParser::compareAndSubstring(std::string method, std::string &requestLine)
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
	if (requestLine.compare(HTTP_VERSION) == 0)
		throw BadRequestException("Wrong Http version");
	return HTTP_VERSION;
}

const std::string HttpRequestParser::parseMethodStr(std::string &requestLine)
{
	std::string method;
	size_t		pos;

	pos = requestLine.find(' ');
	method = requestLine.substr(0, pos);
	requestLine = requestLine.substr(pos + 1, requestLine.length());
	return method;
}

const std::string HttpRequestParser::parseUri(std::string &requestLine)
{
	std::string uri;

	size_t uriPos = requestLine.find('?');
	size_t paramPos = requestLine.find(' ');
	if (uriPos < paramPos)
		uri = requestLine.substr(0, uriPos);
	else
		uri = requestLine.substr(0, paramPos);

	requestLine = requestLine.substr(paramPos, requestLine.length());
	return uri;
}

const std::string HttpRequestParser::getHeaderValue(std::map<std::string, std::string> &headers, std::string toFind)
{
	std::map<std::string, std::string>::iterator it;

	it = headers.find(toFind);
	if (it == headers.end())
		throw BadRequestException("Header not found");
	return it->second;
}

void HttpRequestParser::parseHeader(const std::string &line, std::map<std::string, std::string> &headers)
{
	size_t pos = line.find(':');
	if (pos != std::string::npos)
	{
		std::string key = line.substr(0, pos);
		std::string value = line.substr(pos + 2);
		value.replace(value.length() - 1, value.length(), "");

		std::pair<std::map<std::string, std::string>::iterator, bool> result;
		result = headers.insert(std::pair<std::string, std::string>(key, value));
		if (result.second == false)
			throw BadRequestException("Duplicate header found");
	}
}

void HttpRequestParser::findBody(std::string newLine, bool &bodyFound)
{
	if (bodyFound == false && newLine.compare(HTTP_LINEBREAK))
		bodyFound = true;
}

void HttpRequestParser::parseBody(std::string newLine, std::string &body)
{
	newLine.replace(newLine.length() - 1, newLine.length(), "");
	body.append(newLine);
}

