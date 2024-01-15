#include "HttpRequestParser.hpp"

HttpRequestParser::HttpRequestParser() {}

HttpRequestParser::~HttpRequestParser() {}

HttpRequest HttpRequestParser::parseHttpRequest(std::string requestInput)
{
	std::stringstream					ss(requestInput);
	std::string							requestLine = "";
	std::string							newLine = "";
	std::string							method = "";
	std::string							uri = "";
	std::string							version = "";
	std::string							body = "";
	std::string							host = "";
	int									lineBreak = 0;
	std::string							contentLength = "";
	std::map<std::string, std::string>	headers;
	unsigned long requestReady = std::stol(contentLength);

	while (getline(ss, newLine, '\n'))
	{
		if (requestLine.empty())
		{
			requestLine = newLine;
			parseRequestLine(requestLine, method, uri, version);
		}
		else if ((lineBreak == 0 || lineBreak == 1) && newLine.compare(HTTP_LINEBREAK) == 0)
		{
			lineBreak += 1;
			continue ;
		}
		else if (lineBreak == 0)
			parseHeaders(newLine, headers);
		if (lineBreak == 2)
		{
			if (method.compare("POST") == 0)
			{
				if (contentLength.empty())
					contentLength = getHeaderValue(headers, "Content-Length");
				parseBody(newLine, body);
				if (requestReady == body.length())
					break;
			}
			else
				break;
		}
	}
	host = getHeaderValue(headers, "Host");
	HttpRequest request(method, version, uri, host, body, body.length());
	return request;
}

void HttpRequestParser::parseRequestLine(std::string &requestLine, std::string &method, std::string &uri, std::string &version)
{
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

const std::string HttpRequestParser::parseMethod(std::string &requestLine)
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

