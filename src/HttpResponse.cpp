
#include "../include/HttpResponse.hpp"
#include "HttpResponse.hpp"

const std::string	HttpResponse::_version = "HTTP/1.1";

HttpResponse::HttpResponse(void) : _date(Timer::GetTimeDate()),  _contentType("txt/html"), _contentLenght(0)
{
}

HttpResponse::HttpResponse(const std::string date,
						   const std::string contentType)
	: _date(date),
	  _contentType(contentType),
	  _contentLenght(0)
{
}

HttpResponse::~HttpResponse(void)
{
}

HttpResponse::HttpResponse(const HttpResponse &rhs)
	: _date(rhs._date),
	  _contentType(rhs._contentType),
	  _contentLenght(rhs._contentLenght),
	  _status(rhs._status),
	  _body(rhs._body)
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
	this->_contentLenght = strlen(this->_body.c_str());
}

void HttpResponse::setStatus(const unsigned int code, const std::string message)
{
	this->_status.code = code;
	this->_status.message = message;
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
