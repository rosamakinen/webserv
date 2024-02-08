#include "HttpRequestParser.hpp"

HttpRequestParser::HttpRequestParser() {}

HttpRequestParser::~HttpRequestParser() {}

HttpRequest *HttpRequestParser::parseHttpRequest(std::string requestInput, Server *server)
{
	HttpRequest *request = new HttpRequest();
	std::stringstream ss(requestInput);
	std::string requestLine;

	// Parse the request line
	getline(ss, requestLine);
	parseRequestLine(requestLine, request, server);

	// Parse the headers
	while (getline(ss, requestLine))
	{
		if (requestLine.compare("\r") == 0)
			break;
		parseHeader(requestLine, request);
	}

	std::string	body = "";
	while (getline(ss, requestLine))
	{
		if (requestLine.compare("\r") == 0)
			break;
		request->appendBody(requestLine);
	}
	request->setHost(request->getHeader("Host"));
	return request;
}

void HttpRequestParser::parseRequestLine(std::string &requestLine, HttpRequest *request, Server *server)
{
	if (requestLine.empty())
		throw BadRequestException("Empty requestline");
	parseMethod(requestLine, request);
	parseUri(requestLine, request);
	parseDirectoryAndLocation(request, server);
	validateMethod(request, server);
	parseIndexPathAndDirectoryListing(request, server);
	parseCgiMethod(request);
	validateVersion(requestLine);
}

void HttpRequestParser::parseDirectoryAndLocation(HttpRequest *request, Server *server)
{
	std::string directoryPath = Util::getDirectoryFromUri(request->getUri());
	if (!server->isLocationInServer(directoryPath))
		throw NotFoundException("Location does not exist");
	request->setLocation(directoryPath);

	const std::vector<std::string>* workingDir = server->getLocationValue(directoryPath, LOCAL_DIR);
	if (workingDir == nullptr && workingDir->size() != 1)
		throw BadRequestException("Location has missing or invalid values");

	request->setDirectory(workingDir->front());
}

void HttpRequestParser::validateMethod(HttpRequest *request, Server *server)
{
	const std::vector<std::string> *values = server->getLocationValue(request->getLocation(), HTTP_METHOD);
	if (values == nullptr || values->size() < 1)
		throw MethodNotAllowedException("Requested method is not allowed for the location");

	Util::METHOD method = request->getMethod();
	for (std::vector<std::string>::const_iterator it = values->begin(); it != values->end(); it++)
	{
		if (it->compare(Util::translateMethod(method)) == 0)
			return;
	}

	throw MethodNotAllowedException("Requested method is not allowed for the location");
}

void HttpRequestParser::parseMethod(std::string &requestLine, HttpRequest *request)
{
	Util::METHOD method = Util::METHOD::NONE;
	if (compareAndSubstring("GET ", requestLine) == 0)
		method = Util::METHOD::GET;
	else if (compareAndSubstring("POST ", requestLine) == 0)
		method = Util::METHOD::POST;
	else if (compareAndSubstring("DELETE ", requestLine) == 0)
		method = Util::METHOD::DELETE;
	else
		parseMethodStr(requestLine);

	request->setMethod(method);
}

void HttpRequestParser::parseIndexPathAndDirectoryListing(HttpRequest *request, Server *server)
{
	std::string uri = request->getUri();
	if (uri[uri.length() - 1] == '/')
	{
		const std::vector<std::string> *indexValues = server->getLocationValue(request->getLocation(), INDEX);
		if (indexValues != nullptr)
		{
			std::string indexPath = request->getDirectory();
			indexPath.append(indexValues->front());
			request->setResourcePath(indexPath);
			return;
		}

		const std::vector<std::string> *autoIndexValues = server->getLocationValue(request->getLocation(), AUTO_INDEX);
		if (autoIndexValues != nullptr && autoIndexValues->size() >= 1 && autoIndexValues->front().compare("true") == 0)
		{
			request->setResourcePath(request->getDirectory());
			request->setIsDirListing(true);
			return;
		}

		throw ForbiddenException("Autoindexing not enabled for requested resource");
	}
	else
	{
		std::string indexPath = request->getDirectory();
		indexPath.append(Util::getFileFromUri(request->getUri()));
		request->setResourcePath(indexPath); // leak comes after setting the ResourcePath
	}
}

void HttpRequestParser::parseCgiMethod(HttpRequest *request)
{
	Util::METHOD method = request->getMethod();
	std::string path = request->getResourcePath();

	if (!findCgi(path))
		return ;

	if (method == Util::METHOD::GET)
		request->setMethod(Util::METHOD::CGI_GET);
	else if (method == Util::METHOD::POST)
		request->setMethod(Util::METHOD::CGI_POST);
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

bool HttpRequestParser::validateCgi(std::string path)
{
	std::string suffix = ".py";
	std::string fullPath = FileHandler::getFilePath(path);
	if (access(fullPath.c_str(), X_OK) == 0)
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

void HttpRequestParser::validateVersion(std::string &requestLine)
{
	if (requestLine.compare(HTTP_VERSION) == 0)
		throw BadRequestException("Unsupported HTTP version");
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

void HttpRequestParser::parseParameters(std::string uri, std::map<std::string, std::string>& parameters)
{
	std::stringstream input(uri.append("&"));
	std::string segment;
	while (std::getline(input, segment, '&'))
	{
		size_t pos = segment.find('=');
		std::string key = segment.substr(0, pos);
		std::string value = segment.substr(pos + 1, segment.length());
		if (pos == std::string::npos || key.empty() || value.empty())
			throw BadRequestException("Invalid request parameter");

		std::pair<std::map<std::string, std::string>::iterator, bool> result;
		result = parameters.insert(std::pair<std::string, std::string>(key, value));
		if (result.second == false)
			throw BadRequestException("Duplicate request parameter");
	}
}

void HttpRequestParser::parseUri(std::string &requestLine, HttpRequest *request)
{
	std::string uri;
	std::map<std::string, std::string> parameters;
	size_t uriPos = requestLine.find('?');
	size_t paramPos = requestLine.find(' ');
	if (uriPos < paramPos)
	{
		uri = requestLine.substr(0, uriPos);
		requestLine = requestLine.substr(uriPos + 1, requestLine.length());
		parseParameters(requestLine.substr(0, requestLine.find(' ')), parameters);
	}
	else
	{
		uri = requestLine.substr(0, paramPos);
		requestLine = requestLine.substr(paramPos, requestLine.length());
	}

	request->setUri(uri);
	request->setParameters(parameters);
}

const std::string HttpRequestParser::getHeaderValue(std::map<std::string, std::string> &headers, std::string toFind)
{
	std::map<std::string, std::string>::iterator it;

	it = headers.find(toFind);
	if (it == headers.end())
		throw BadRequestException("Header not found");
	return it->second;
}

void HttpRequestParser::parseHeader(const std::string &line, HttpRequest *request)
{
	size_t pos = line.find(':');
	if (pos != std::string::npos)
	{
		std::string key = line.substr(0, pos);
		std::string value = line.substr(pos + 2);
		value.replace(value.length() - 1, value.length(), "");

		if (!request->setHeader(key, value))
			throw BadRequestException("Duplicate header found");
	}
}

void HttpRequestParser::parseBody(std::string newLine, std::string &body)
{
	newLine.replace(newLine.length() - 1, newLine.length(), "");
	body.append(newLine);
}

