

#include "../include/HttpRequest.hpp"
#include "HttpRequest.hpp"

HttpRequest::HttpRequest() : method("GET"), version("HTTP/1.1"), uri("/"), host("localhost"), body(""), contentLenght(0)
{
}

HttpRequest::HttpRequest(
	const std::string& method,
	const std::string& version,
	const std::string& uri,
	const std::string& host,
	const std::string body,
	const int& contentLenght
	) : method(method),
	version(version),
	uri(uri),
	host(host),
	body(body),
	contentLenght(contentLenght)
{
	isValid(*this);
}

HttpRequest::HttpRequest(const HttpRequest& rhs) :
	method(rhs.method),
	version(rhs.version),
	uri(rhs.uri),
	host(rhs.host),
	body(rhs.body),
	contentLenght(rhs.contentLenght)
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
	return this->method;
}

const std::string			HttpRequest::getVersion() const
{
	return this->version;
}

const std::string			HttpRequest::getUri() const
{
	return this->uri;
}

const std::string			HttpRequest::getHost() const
{
	return this->host;
}

const std::string			HttpRequest::getBody() const
{
	return this->body;
}

const int&					HttpRequest::getContentLenght() const
{
	return this->contentLenght;
}

const std::string	HttpRequest::allowedVersion = "HTTP/1.1";
const std::string	HttpRequest::allowedMethods[] =
{
	"GET",
	"POST"
};

void HttpRequest::isValid(const HttpRequest& request)
{
	if (request.contentLenght <= 0)
		std::cerr << "Request content lenght must be more than 0" << std::endl;

	if (request.version.compare(HttpRequest::allowedVersion) != 0)
		std::cerr << "Request version not allowed" << std::endl;

	bool allowed = false;
	for (size_t i = 0; i < allowedMethods->size(); i++)
	{
		if (allowedMethods[i].compare(request.method) == 0)
			allowed = true;
	}
	if (!allowed)
		std::cerr << "Request method not allowed" << std::endl;

	if (request.method.compare(HttpRequest::allowedMethods[1]) == 0
		&& request.body.length() == 0)
		std::cerr << "POST Request needs to have a body" << std::endl;
}
