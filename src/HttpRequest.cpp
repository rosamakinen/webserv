

#include "../include/HttpRequest.hpp"
#include "HttpRequest.hpp"

// HttpRequest::HttpRequest() : _method("GET"), _version("HTTP/1.1"), _uri("/"), _host("localhost"), _body(""), _contentLength(0)
// {
// }

HttpRequest::HttpRequest(
	const std::string& method,
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
	// isValid(*this);
}

HttpRequest::HttpRequest(const HttpRequest& rhs) :
	_method(rhs._method),
	_version(rhs._version),
	_uri(rhs._uri),
	_host(rhs._host),
	_body(rhs._body),
	_contentLength(rhs._contentLength)
{
}

HttpRequest::~HttpRequest()
{
}

HttpRequest &HttpRequest::operator=(const HttpRequest& rhs)
{
	// All fields are const so nothing to do here

	if (this == &rhs)
		return *this;
	return *this;
}

const std::string			HttpRequest::getMethod() const
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

const std::string	HttpRequest::_allowedVersion = "HTTP/1.1";
const std::string	HttpRequest::_allowedMethods[] =
{
	"GET",
	"POST"
};

// void HttpRequest::isValid(const HttpRequest& request)
// {
// 	if (request._contentLength <= 0)
// 		std::cerr << "Request content length must be more than 0" << std::endl;

// 	if (request._version.compare(HttpRequest::_allowedVersion) != 0)
// 		std::cerr << "Request version not allowed" << std::endl;

// 	bool allowed = false;
// 	for (size_t i = 0; i < _allowedMethods->size(); i++)
// 	{
// 		if (_allowedMethods[i].compare(request._method) == 0)
// 			allowed = true;
// 	}
// 	if (!allowed)
// 		std::cerr << "Request method not allowed" << std::endl;

// 	if (request._method.compare(HttpRequest::_allowedMethods[1]) == 0
// 		&& request._body.length() == 0)
// 		std::cerr << "POST Request needs to have a body" << std::endl;
// }
