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
		this->method = rhs.method;
		this->version = rhs.version;
		this->uri = rhs.uri;
		this->host = rhs.host;
		this->body = rhs.body;
		this->contentLength = rhs.contentLength;
		this->requestLine = rhs.requestLine;
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
		if (this->requestLine.empty())
			parseRequestLine(newLine);
		// else if (headers)
		// else (body)
	}
	this->host = parseHost();
	this->body = parseBody();
	this->contentLength = parseContentLength();

	HttpRequest request(method, version, uri, host, body, contentLength);
	return request;
}

void HttpRequestParser::parseRequestLine(std::string requestLine)
{
	if (this->method.empty())
		this->method = parseMethod(requestLine);
	else if (this->uri.empty())
		this->uri = parseUri(requestLine);
	else if (this->version.empty())
		this->version = parseVersion(requestLine);
}

const std::string HttpRequestParser::parseMethod(std::string requestLine)
{
	std::string tempMethod = "";
	if (requestLine.compare(0, 4, "GET "))
		tempMethod = "GET";
	else if (requestLine.compare(0, 5, "POST "))
		tempMethod = "POST";
	else if (requestLine.compare(0, 7, "DELETE "))
		tempMethod = "DELETE";
	else
		std::cout << "Wrong method type" << std::endl;

	return tempMethod;
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
