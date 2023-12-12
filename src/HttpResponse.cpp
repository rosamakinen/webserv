
#include "../include/HttpResponse.hpp"
#include "HttpResponse.hpp"

const std::string	HttpResponse::version = "HTTP/1.1";

HttpResponse::HttpResponse(void) : date("1.1.1990"), serverName("localhost"), contentLenght(1000), contentType("txt/html"), statusCode(200)
{
}

HttpResponse::HttpResponse(const std::string date,
						   const std::string serverName,
						   const unsigned int contentLenght,
						   const std::string contentType,
						   const unsigned int statusCode)
	: date(date),
	  serverName(serverName),
	  contentLenght(contentLenght),
	  contentType(contentType),
	  statusCode(statusCode)
{
}

HttpResponse::~HttpResponse(void)
{
}

HttpResponse::HttpResponse(const HttpResponse &rhs)
	: date(rhs.date),
	  serverName(rhs.serverName),
	  contentLenght(rhs.contentLenght),
	  contentType(rhs.contentType),
	  statusCode(rhs.statusCode)
{
	*this = rhs;
}

HttpResponse &HttpResponse::operator=(const HttpResponse &rhs)
{
	if (this != &rhs)
		return *this;
	return *this;
}

void HttpResponse::setBody(const std::string body)
{
	this->body = body;
}

const std::string HttpResponse::getDate() const
{
	return this->date;
}

const std::string HttpResponse::getName() const
{
	return this->serverName;
}

unsigned int HttpResponse::getContentLenght() const
{
	return this->contentLenght;
}

const std::string HttpResponse::getContentType() const
{
	return this->contentType;
}

const std::string HttpResponse::getBody() const
{
	return this->body;
}

unsigned int HttpResponse::getStatusCode() const
{
	return this->statusCode;
}
