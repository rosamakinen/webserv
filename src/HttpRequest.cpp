#include "../include/HttpRequest.hpp"

HttpRequest::HttpRequest() : _version("HTTP/1.1"), _isDirListing(false)
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

void HttpRequest::appendBody(std::string body)
{
	this->_body.append(body);
}

void HttpRequest::setContentLength(size_t contentLength)
{
	this->_contentLength = contentLength;
}

void HttpRequest::setIsDirListing(bool isDirListing)
{
	this->_isDirListing = isDirListing;
}

bool HttpRequest::getIsDirListing()
{
	return this->_isDirListing;
}

int HttpRequest::getPort()
{
	return this->_port;
}

std::string HttpRequest::getServerName()
{
	return this->_server_name;
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
