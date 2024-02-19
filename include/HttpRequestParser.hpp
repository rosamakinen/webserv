#ifndef HTTPREQUESTPARSER_HPP
#define HTTPREQUESTPARSER_HPP

#include "WebServer.hpp"
#include "HttpRequest.hpp"
#include "Server.hpp"
#include "FileHandler.hpp"

class HttpRequestParser
{
	private:
		void parseParameters(std::string uri, std::map<std::string, std::string>& parameters);
		void parseUri(std::string &requestLine, HttpRequest *request);
		void parseRequestLine(std::string &requestLine, HttpRequest *request, Server *server);
		void parseDirectoryAndLocation(HttpRequest *request, Server *server);
		void validateMethod(HttpRequest *request, Server *server);
		void parseMethod(std::string &requestLine, HttpRequest *request);
		void parseIndexPathAndDirectoryListing(HttpRequest *request, Server *server);
		void parseHeader(const std::string &line, HttpRequest *request);
		void validateVersion(std::string &requestLine);
		void parseMethodStr(std::string &requestLine);
		const std::string getHeaderValue(std::map<std::string, std::string> &headers, std::string toFind);
		void parseBody(std::string newLine, std::string &body);
		int compareAndSubstring(std::string method, std::string &requestLine);
		void parseCgiMethod(HttpRequest *request);
		bool findCgi(std::string uri);
		bool validateCgi(std::string path);
		void parseContentLenght(HttpRequest *request);
		void validateSize(HttpRequest *request, Server *server);

	public:
		HttpRequestParser();
		~HttpRequestParser();

		HttpRequest	*parseHttpRequest(std::string request, Server *server);
};

#endif
