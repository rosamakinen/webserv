#ifndef HTTPREQUEST_HPP
#define HTTPREQUEST_HPP

#include "Util.hpp"
#include "WebServer.hpp"
#include "Exceptions.hpp"

class HttpRequest
{
	public:
		HttpRequest(void);
		~HttpRequest(void);

		Util::METHOD getMethod() const;
		const std::string getUri() const;
		const std::string getHost() const;
		const std::string getBody() const;
		std::string getDirectory() const;
		std::string getLocation() const;
		std::string getResourcePath() const;
		std::string getContentType() const;
		size_t getContentLength() const;
		std::map<std::string, std::string> getParameters();
		std::map<std::string, std::string> getHeaders();
		const std::string getHeader(std::string key);
		bool getIsDirListing();
		int getPort();
		std::string getServerName();

		void setIsDirListing(bool isDirListing);
		void setMethod(Util::METHOD method);
		void setUri(std::string uri);
		void setHost(std::string host);
		void appendBody(std::string body);
		void setContentType(std::string type);
		void setContentLength(size_t contentLength);
		void setParameters(std::map<std::string, std::string> parameters);
		bool setHeader(std::string key, std::string value);
		void setDirectory(std::string directoryPath);
		void setLocation(std::string location);
		void setResourcePath(std::string path);
		void setPort(int port);
		void setServerName(std::string name);

	private:
		static const std::string	_allowedMethods[];

		Util::METHOD _method;
		std::string _version;
		std::string _uri;
		std::string _host;
		std::string _body;
		std::string _resourcePath;
		std::string _directory;
		std::string _location;
		std::string _contentType;
		std::string _server_name;
		int _port;
		size_t _contentLength;
		bool _isDirListing;
		std::map<std::string, std::string> _parameters;
		std::map<std::string, std::string> _headers;
};

#endif
