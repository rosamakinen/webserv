
#include "../include/HttpRequest.hpp"

HttpRequest::HttpRequest(
	HttpRequest::METHOD method,
	const std::string& version,
	const std::string& uri,
	const std::string& host,
	const std::string body,
	const int& contentLength
	) : _method(method),
	_version(version),
	_uri(uri),
	_host(host),
	_body(body),
	_contentLength(contentLength)
{
}

HttpRequest::~HttpRequest()
{
}

HttpRequest::METHOD	HttpRequest::getMethod() const
{
	return this->_method;
}

const std::string			HttpRequest::getVersion() const
{
	return this->_version;
}

const std::string			HttpRequest::getUri() const
{
	return this->_uri;
}

const std::string			HttpRequest::getHost() const
{
	return this->_host;
}

const std::string			HttpRequest::getBody() const
{
	return this->_body;
}

const int&					HttpRequest::getContentLength() const
{
	return this->_contentLength;
}
