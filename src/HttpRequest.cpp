#include "../include/HttpRequest.hpp"

HttpRequest::HttpRequest() : _version("HTTP/1.1"), _isDirListing(false), _isRedirected(false)
{
}

HttpRequest::~HttpRequest()
{
	_headers.clear();
	_parameters.clear();
}

void HttpRequest::setMethod(Util::METHOD method)
{
	this->_method = method;
}

void HttpRequest::setDirectory(std::string directoryPath)
{
	this->_directory = directoryPath;
}

void HttpRequest::setUri(std::string uri)
{
	this->_uri = uri;
}

void HttpRequest::setHost(std::string host)
{
	this->_host = host;
}

void HttpRequest::setLocation(std::string location)
{
	this->_location = location;
}

void HttpRequest::setResourcePath(std::string path)
{
	this->_resourcePath = path;
}

void HttpRequest::setPort(int port)
{
	this->_port = port;
}

void HttpRequest::setServerName(std::string name)
{
	this->_server_name = name;
}

void HttpRequest::setFileName(std::string fileName)
{
	this->_filename = fileName;
}

void HttpRequest::setRedirLocation(std::string redirLocation)
{
	this->_redirLocation = redirLocation;
}

void HttpRequest::appendBody(std::string body)
{
	this->_body.append(body);
}

void HttpRequest::setContentType(std::string type)
{
	this->_contentType = type;
}
void HttpRequest::setContentLength(size_t contentLength)
{
	this->_contentLength = contentLength;
}
void HttpRequest::setBodyLength(size_t bodyLength)
{
	this->_bodyLength = bodyLength;
}

void HttpRequest::setIsDirListing(bool isDirListing)
{
	this->_isDirListing = isDirListing;
}

bool HttpRequest::getIsDirListing()
{
	return this->_isDirListing;
}

void HttpRequest::setIsRedirected(bool isRedirected)
{
	this->_isRedirected = isRedirected;
}

bool HttpRequest::getIsRedirected()
{
	return this->_isRedirected;
}

int HttpRequest::getPort()
{
	return this->_port;
}

std::string HttpRequest::getServerName()
{
	return this->_server_name;
}

std::string HttpRequest::getFileName()
{
	return this->_filename;
}

Util::METHOD HttpRequest::getMethod() const
{
	return this->_method;
}

std::string HttpRequest::getLocation() const
{
	return this->_location;
}

std::string HttpRequest::getResourcePath() const
{
	return this->_resourcePath;
}

std::string HttpRequest::getContentType() const
{
	return this->_contentType;
}

std::string HttpRequest::getRedirLocation() const
{
	return this->_redirLocation;
}

std::string HttpRequest::getDirectory() const
{
	return this->_directory;
}

const std::string HttpRequest::getUri() const
{
	return this->_uri;
}

const std::string HttpRequest::getHost() const
{
	return this->_host;
}

const std::string HttpRequest::getBody() const
{
	return this->_body;
}

size_t HttpRequest::getContentLength() const
{
	return this->_contentLength;
}
size_t HttpRequest::getBodyLength() const
{
	return this->_bodyLength;
}

void HttpRequest::setParameters(std::map<std::string, std::string> parameters)
{
	this->_parameters = parameters;
}

std::map<std::string, std::string> HttpRequest::getParameters()
{
	return this->_parameters;
}

std::map<std::string, std::string> HttpRequest::getHeaders()
{
	return this->_headers;
}

bool HttpRequest::setHeader(std::string key, std::string value)
{
	std::pair<std::map<std::string, std::string>::iterator, bool> result;
	result = this->_headers.insert(std::pair<std::string, std::string>(key, value));
	return result.second;
}

const std::string HttpRequest::getHeader(std::string key)
{
	std::map<std::string, std::string>::iterator it = this->_headers.find(key);
	if (it == this->_headers.end())
		return "";
	return it->second;
}
