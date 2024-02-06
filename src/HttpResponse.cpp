
#include "../include/HttpResponse.hpp"
#include "HttpResponse.hpp"

std::map<std::string, std::string> _contenttypes =
{
	std::make_pair(EXT_HTML, "text/html; charset=utf-8"),
	std::make_pair(EXT_CSS, "text/css"),
	std::make_pair(EXT_JPEG, "image/jpeg")
};

bool HttpResponse::contentTypeSet(std::string resourcePath, std::string contentTypeToFind)
{
	int len = resourcePath.length();
	if (len <= 1)
		return false;

	std::map<std::string, std::string>::const_iterator it = _contenttypes.find(contentTypeToFind);
	if (resourcePath.substr(len - it->first.length(), len).compare(it->first) == 0)
	{
		this->_contentType = it->second;
		return true;
	}

	return false;
}

std::ios_base::openmode HttpResponse::setContentType(std::string resourcePath)
{
	if (contentTypeSet(resourcePath, EXT_HTML))
		return std::ifstream::in;
	else if (contentTypeSet(resourcePath, EXT_CSS))
		return std::ifstream::in;
	else if (contentTypeSet(resourcePath, EXT_JPEG))
		return std::ifstream::binary;
	return std::ifstream::in;
}

HttpResponse::HttpResponse(
	const std::pair<unsigned int, std::string> &status,
	HttpRequest *request,
	Server *server)
	: _contentLenght(0),
	_status(status)
{
	if (this->getStatus().first != 200)
		setBody(FileHandler::getErrorFileContent(status.first, server));
	else
	{
		// TODO add redirection from configurated root to the index html
		std::ios_base::openmode mode = setContentType(request->getResourcePath());
		setBody(FileHandler::getFileResource(request, mode));
	}
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
