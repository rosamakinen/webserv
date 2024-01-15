#include "HttpRequestParser.hpp"

HttpRequestParser::HttpRequestParser() {}

HttpRequestParser::~HttpRequestParser() {}

HttpRequest HttpRequestParser::parseHttpRequest(std::string requestInput)
{
	std::stringstream					ss(requestInput);
	// std::string							newLine = "";
	// std::string							body = "";
	// std::string							host = "";
	// std::string							contentLength;
	// std::map<std::string, std::string>	headers;
	// int									lineBreak = 0;

	std::string requestLine, uri, version;
	HttpRequest::METHOD method;
	if (getline(ss, requestLine))
		parseRequestLine(requestLine, method, uri, version);


	host = getHeaderValue(headers, "Host");
	HttpRequest request(method, version, uri, host, body, body.length());
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
	if (compareMethod("GET ", requestLine) == 0)
		return HttpRequest::METHOD::GET;
	else if (compareMethod("POST ", requestLine) == 0)
		return HttpRequest::METHOD::POST;
	else if (compareMethod("DELETE ", requestLine) == 0)
		return HttpRequest::METHOD::DELETE;
	else
		throw BadRequestException("Wrong method type");
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
	if (requestLine.compare(HTTP_VERSION) == 0)
		throw BadRequestException("Wrong Http version");
	return HTTP_VERSION;
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

const std::string HttpRequestParser::getHeaderValue(std::map<std::string, std::string> &headers, std::string toFind)
{
	std::map<std::string, std::string>::iterator it;
	std::string ret = "";

	it = headers.find(toFind);
	if (it == headers.end())
		throw BadRequestException("Header not found");
	else
		ret = it->second;

	return ret;
}

void HttpRequestParser::parseHeaders(const std::string &line, std::map<std::string, std::string> &headers)
{
	size_t pos = line.find(':');
	if (pos != std::string::npos)
	{
		std::string key = line.substr(0, pos);
		std::string value = line.substr(pos + 1);

		if (headers[key].empty())
			headers[key] = value;
	}

}

void HttpRequestParser::findBody(std::string newLine, bool &bodyFound)
{
	if (bodyFound == false && newLine.compare("\r\n"))
		bodyFound = true;
}

void HttpRequestParser::parseBody(std::string newLine, std::string &body)
{
	//TODO: here to check if the body.length == content-length
	//if yes, trigger flag, if not add newLine to body
	body += newLine;
}

