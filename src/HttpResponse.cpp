
#include "../include/HttpResponse.hpp"
#include "HttpResponse.hpp"

const std::string	HttpResponse::_version = "HTTP/1.1";

HttpResponse::HttpResponse(void) : _date("1.1.1990"), _contentLenght(1000), _contentType("txt/html")
{
}

HttpResponse::HttpResponse(const std::string date,
						   const unsigned int contentLenght,
						   const std::string contentType)
	: _date(date),
	  _contentLenght(contentLenght),
	  _contentType(contentType)
{
}

HttpResponse::~HttpResponse(void)
{
}

HttpResponse::HttpResponse(const HttpResponse &rhs)
	: _date(rhs._date),
	  _contentLenght(rhs._contentLenght),
	  _contentType(rhs._contentType)
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
	this->_body = body;
}

void HttpResponse::setStatus(std::pair<unsigned int, std::string> status)
{
	this->_status = status;
}

const std::string HttpResponse::getDate() const
{
	return this->_date;
}

unsigned int HttpResponse::getContentLenght() const
{
	return this->_contentLenght;
}

const std::string HttpResponse::getContentType() const
{
	return this->_contentType;
}

const std::string HttpResponse::getBody() const
{
	return this->_body;
}
