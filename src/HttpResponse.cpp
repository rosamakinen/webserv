
#include "../include/HttpResponse.hpp"
#include "../include/FileHandler.hpp"

HttpResponse::HttpResponse(
	const std::pair<unsigned int, std::string> &status,
	const std::string& resourcePath)
	: _contentType("text/html; charset=utf-8"),
	_contentLenght(0),
	_status(status)
{
	if (status.first != 200)
		setBody(FileHandler::getErrorFileContent(status.first));
	else
		setBody(FileHandler::getFileResource(resourcePath));
}

HttpResponse::~HttpResponse(void)
{
}

void HttpResponse::setBody(const std::string body)
{
	this->_body = body;
	this->_contentLenght = this->_body.length();
}

void HttpResponse::setStatus(const std::pair<unsigned int, std::string> &status)
{
	this->_status = status;
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

const std::pair<unsigned int, std::string> HttpResponse::getStatus() const
{
	return this->_status;
}
