#include "HttpRequestParser.hpp"

HttpRequestParser::HttpRequestParser() {}

HttpRequestParser::~HttpRequestParser() {}

HttpRequest *HttpRequestParser::parseHttpRequest(std::string requestInput, Server *server)
{
	std::stringstream					ss(requestInput);
	std::string requestLine, uri, version;
	HttpRequest::METHOD method;
	getline(ss, requestLine);
	parseRequestLine(requestLine, method, uri, version, server);

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
		parseBody(requestLine, body);
	}

	std::string host = getHeaderValue(headers, "Host");
	HttpRequest *request = new HttpRequest(method, version, uri, host, "body", 14);
	return request;
}

void HttpRequestParser::parseRequestLine(
	std::string &requestLine,
	HttpRequest::METHOD& method,
	std::string &uri,
	std::string &version,
	Server *server)
{
	if (requestLine.empty())
		throw BadRequestException("Empty requestline");

	method = parseMethod(requestLine);
	uri = parseUri(requestLine);
	validateMethod(uri, method, server);
	parseCgiMethod(method, uri);
	version = parseVersion(requestLine);
}

void HttpRequestParser::validateMethod(std::string& uri, HttpRequest::METHOD method, Server *server)
{
	std::cout << "Looking for location: '" << uri << "' for method '" << method << "'" << std::endl;
	const std::vector<std::string> *values = server->getLocationValue(uri, HTTP_METHOD);
	if (values != nullptr && values->size() >= 1)
		throw MethodNotAllowedException("Requested method is not allowed for the location");

	for (std::string value : *values)
	{
		if (value.compare(CONFIG_TRUE) == 0)
			return;
	}

	throw MethodNotAllowedException("Requested method is not allowed for the location");
}

HttpRequest::METHOD HttpRequestParser::parseMethod(std::string &requestLine)
{
	if (compareAndSubstring("GET ", requestLine) == 0)
		return HttpRequest::METHOD::GET;
	else if (compareAndSubstring("POST ", requestLine) == 0)
		return HttpRequest::METHOD::POST;
	else if (compareAndSubstring("DELETE ", requestLine) == 0)
		return HttpRequest::METHOD::DELETE;
	else
	{
		parseMethodStr(requestLine);
		return HttpRequest::METHOD::NONE;
	}
}

void HttpRequestParser::parseCgiMethod(HttpRequest::METHOD &method, std::string &uri)
{
	if (findCgi(uri) == true)
	{
		if (method == HttpRequest::METHOD::GET)
		{
			method = HttpRequest::METHOD::CGI_GET;
			return ;
		}
		if (method == HttpRequest::METHOD::POST)
		{
			method = HttpRequest::METHOD::CGI_POST;
			return ;
		}
	}
}

bool HttpRequestParser::findCgi(std::string uri)
{
	size_t found = uri.find(CGI_LOCATION);
	if (found != std::string::npos)
	{
		bool ret = validateCgi(uri);
		return ret;
	}
	return false;
}

bool HttpRequestParser::validateCgi(std::string uri)
{
	std::string suffix = ".py";
	std::string fullPath = FileHandler::getFilePath(uri);

	if (access(fullPath.c_str(), F_OK) == 0)
	{
		size_t pos = fullPath.find(suffix);
		if (pos != std::string::npos)
			return true;
	}
	throw BadRequestException("Bad CGI request");
	return false;
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

