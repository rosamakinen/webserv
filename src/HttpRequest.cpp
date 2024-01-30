
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

std::string 				HttpRequest::translateMethod(HttpRequest::METHOD method) const
{
	switch (method)
	{
		case HttpRequest::METHOD::GET :
			return HTTP_GET;
		break;

		case HttpRequest::METHOD::POST :
			return HTTP_POST;
		break;

		case HttpRequest::METHOD::DELETE :
			return HTTP_DELETE;
		break;

		case HttpRequest::METHOD::CGI_GET :
			return HTTP_GET;
		break;

		case HttpRequest::METHOD::CGI_POST :
			return HTTP_POST;
		break;
		
		default:
		break;
	}
	return nullptr;
}