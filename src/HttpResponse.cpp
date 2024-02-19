#include "../include/HttpResponse.hpp"

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

HttpResponse::~HttpResponse(void)
{
}

HttpResponse::HttpResponse()
	: _contentLenght(0)
{
}

void HttpResponse::setResponseBody(HttpRequest *request, Server *server)
{
	if (this->getStatus().first != 200)
	{
		this->_contentType = _contenttypes.find(EXT_HTML)->second;
		setBody(FileHandler::getErrorFileContent(this->getStatus().first, server));
	}
	else
	{
		// TODO add redirection from configurated root to the index html
		if (request->getMethod() == Util::METHOD::DELETE)
		{
			// TODO: do we just want to redirect to the "front page instead"
			setContentType(EXT_HTML);
			setDeleteResponse();
			return ;
		}
		if (request->getMethod() == Util::METHOD::POST)
		{
			//parse 201 created here
			setContentType(EXT_HTML);
			setPostResponse();
			return ;
		}
		std::ios_base::openmode mode = setContentType(request->getResourcePath());
		setBody(FileHandler::getFileResource(request, mode));
	}
}

void HttpResponse::setDeleteResponse()
{
	std::string html = "<html><title>YAY</title></head><body><center><h1>file deleted succesfully!</h1></center></body></html>";
	setBody(html);
}

void HttpResponse::setPostResponse()
{
	std::string html = "<html><title>YAY</title></head><body><center><h1>file created succesfully!</h1></center></body></html>";
	setBody(html);
}

void HttpResponse::setCgiResponse(std::string input)
{
	this->_cgiResponse = input;
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

const std::string HttpResponse::getCgiResponse() const
{
	return this->_cgiResponse;
}
