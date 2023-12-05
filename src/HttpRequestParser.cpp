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

HttpRequest &HttpRequestParser::parseHttpRequest(std::string requestString)
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

	//could also do the looping with a megastring that has what
	this->method = parseMethod();
	this->version = parseVersion();
	this->uri = parseUri();
	this->host = parseHost();
	this->body = parseBody();
	this->contentLength = parseContentLength();

	HttpRequest request(method, version, uri, host, body, contentLength);
	return request;
}

void HttpRequestParser::parseRequestLine(std::string requestLine)
{

}

const std::string HttpRequestParser::parseMethod()
{
	this->
	return std::string();
}

const std::string HttpRequestParser::parseVersion()
{
	return std::string();
}

const std::string HttpRequestParser::parseUri()
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
