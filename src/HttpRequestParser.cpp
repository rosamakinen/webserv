#include "HttpRequestParser.hpp"

HttpRequestParser::HttpRequestParser()
{
}

HttpRequestParser::~HttpRequestParser()
{
}

HttpRequestParser::HttpRequestParser(const HttpRequestParser &rhs)
{
	*this = rhs;
}

HttpRequestParser &HttpRequestParser::operator=(const HttpRequestParser &rhs)
{
	if (this != &rhs)
	{
		this->_tempMethod = rhs._tempMethod;
		this->_tempVersion = rhs._tempVersion;
		this->_tempUri = rhs._tempUri;
		this->_tempHost = rhs._tempHost;
		this->_tempBody = rhs._tempBody;
		this->_tempContentLength = rhs._tempContentLength;
		this->_requestLine = rhs._requestLine;
	}
	return *this;
}

HttpRequest HttpRequestParser::parseHttpRequest(std::string requestInput)
{
	std::stringstream	ss(requestInput);
	std::string			newLine;

	while (getline(ss, newLine, '\n'))
	{
		if (this->_requestLine.empty())
		{
			std::cout << "newRequestLine at start: " << newLine << std::endl;
			parseRequestLine(newLine);
		}
		// else if (tempHeaders)
		// else (tempBody)
	}
	// this->_tempHost = parseHost();
	// this->_tempBody = parseBody();
	// this->_tempContentLength = parseContentLength();
	this->_tempHost = "host";
	this->_tempBody = "body";
	this->_tempContentLength = 13;

	HttpRequest request(this->_tempMethod, this->_tempVersion, this->_tempUri, this->_tempHost, this->_tempBody, this->_tempContentLength);
	return request;
}

void HttpRequestParser::parseRequestLine(std::string newLine)
{
	this->_requestLine = newLine;
	parseRequestLineContent();
}

void HttpRequestParser::parseRequestLineContent()
{
	if (this->_requestLine.empty())
	{
		std::cout << "we shouldnt have empty requestline, bro" << std::endl;
		//for debugging, but should we have a check here, altho this should never happen?
	}
	if (this->_tempMethod.empty())
		this->_tempMethod = parseMethod(this->_requestLine);
	if (this->_tempUri.empty())
		this->_tempUri = parseUri(this->_requestLine);
	if (this->_tempVersion.empty())
		this->_tempVersion = parseVersion(this->_requestLine);
}


std::string HttpRequestParser::parseMethod(std::string requestLineInput)
{
	std::string method;
	if (compareMethod("GET ", requestLineInput) == 0)
			method = requestLineInput.substr(0, 4);
	else if (compareMethod("POST ", requestLineInput) == 0)
			method = requestLineInput.substr(0, 5);
	else if (compareMethod("DELETE ", requestLineInput) == 0)
			method = requestLineInput.substr(0, 7);
	else
		std::cout << "Wrong method type" << std::endl; //throw error here
	return method;
}

int	HttpRequestParser::compareMethod(std::string method, std::string requestLineInput)
{
	if (requestLineInput.compare(0, method.length(), method) == 0)
	{
		//TODO: for multiple requests, delete the og request line before substrinning the new one?
		this->_requestLine = requestLineInput.substr(method.length(), requestLineInput.length());
		return 0;
	}
	return 1;
}

const std::string HttpRequestParser::parseVersion(std::string requestLineInput)
{
	std::string version = "HTTP/1.1";
	if (requestLineInput.compare(version) == 0)
	{
			//TODO: throw error
	}
	return version;
}

const std::string HttpRequestParser::parseUri(std::string requestLineInput)
{
	std::string uri;
	size_t		pos;
	pos = requestLineInput.find(' ');
	uri = requestLineInput.substr(0, pos);
	this->_requestLine = requestLineInput.substr(pos, requestLineInput.length());
	return uri;
}

const std::string HttpRequestParser::parseHost()
{
	return std::string();
}

const std::string HttpRequestParser::parseBody()
{
	return std::string();
}

int HttpRequestParser::parseContentLength()
{
	return 1111;
}
