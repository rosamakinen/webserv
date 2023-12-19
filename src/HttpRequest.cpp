
#include "../include/HttpRequest.hpp"

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
	isValid(*this);
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

const std::string	HttpRequest::_allowedMethods[] =
{
	"GET",
	"POST"
};

void HttpRequest::isValid(const HttpRequest& request)
{
	// if (request._contentLength <= 0)
		// throw BadRequestException("Request content length must be more than 0");

	if (request._version.compare(HTTP_VERSION) != 0)
		throw BadRequestException("Request version not allowed");

	bool allowed = false;
	for (size_t i = 0; i < _allowedMethods->size(); i++)
	{
		if (_allowedMethods[i].compare(request._method) == 0)
			allowed = true;
	}
	if (!allowed)
		throw BadRequestException("Request method not allowed");

	// if (request._method.compare(HttpRequest::_allowedMethods[1]) == 0
	// 	&& request._body.length() == 0)
		// throw BadRequestException("POST Request needs to have a body");
}
