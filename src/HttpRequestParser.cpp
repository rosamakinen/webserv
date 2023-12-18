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

HttpRequest &HttpRequestParser::parseHttpRequest(std::string requestInput)
{
	//add requestLine & headers & body variables to class

	//stringstream:: ss, std::string line
	//while with getline(stringstream, line, "\n")
	//check if variable .empty/null
	//if yes/filled, move the line to next checking?

	//compartmentalize to requestLine(method, version, uri), headers and body

	//is contentLength including whitespace? the full thing or just for body

	//with this style i'm going to have issues with const (maybe I could cast them if I want
	// to store them in class, or maybe just store them locally in the function so I dont
	// have to cast them)

	//could also do the looping with a megastring that has the buffer

	std::stringstream	ss(requestInput);
	std::string			newLine;

	while (getline(ss, newLine, '\n'))
	{
		if (this->_requestLine.empty() == 0)
			parseRequestLine(newLine);
		// else if (tempHeaders)
		// else (tempBody)
	}
	this->_tempHost = parseHost();
	this->_tempBody = parseBody();
	this->_tempContentLength = parseContentLength();

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
	if (this->_requestLine.empty() == 0)
	{
		std::cout << "we shouldnt have empty requestline, bro" << std::endl;
		//for debugging, but should we have a check here, altho this should never happen?
	}
	if (this->_tempMethod.empty() == 0)
		this->_tempMethod = parseMethod(this->_requestLine);
	else if (this->_tempUri.empty() == 0)
		this->_tempUri = parseUri(this->_requestLine);
	else if (this->_tempVersion.empty() == 0)
		this->_tempVersion = parseVersion(this->_requestLine);
}


std::string HttpRequestParser::parseMethod(std::string requestLineInput)
{
	std::string method;
	if (compareMethod(4, "GET ", requestLineInput) == 0)
			method = requestLineInput.substr(4, requestLineInput.length());
	else if (compareMethod(5, "POST ", requestLineInput) == 0)
			method = requestLineInput.substr(5, requestLineInput.length());
	else if (compareMethod(7, "DELETE ", requestLineInput) == 0)
			method = requestLineInput.substr(7, requestLineInput.length());
	else
		std::cout << "Wrong method type" << std::endl; //throw error here
	return method;
}

int	HttpRequestParser::compareMethod(int lastIndex, std::string method, std::string requestLineInput)
{
	if (requestLineInput.compare(0, method.length(), method) == 0)
	{
		this->_requestLine = requestLineInput.substr(0, requestLineInput.length());
		std::cout << "it's a match, pal" << this->_requestLine << std::endl; //for debugging
		return 0;
	}
	return 1;
}

const std::string HttpRequestParser::parseVersion(std::string requestLine)
{
	return std::string();
}

const std::string HttpRequestParser::parseUri(std::string requestLine)
{
	return std::string();
}

const std::string HttpRequestParser::parseHost()
{
	return std::string();
}

const std::string HttpRequestParser::parseBody()
{
	return std::string();
}

const int HttpRequestParser::parseContentLength()
{
	return ();
}
