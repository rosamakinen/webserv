
#pragma once

#include "WebServer.hpp"
#include "Exceptions.hpp"

class HttpRequest
{
	public:

		enum METHOD
		{
			NONE,
			GET,
			CGI_GET,
			POST,
			CGI_POST,
			DELETE
		};

		~HttpRequest(void);
		HttpRequest(HttpRequest::METHOD method,
					const std::string& version,
					const std::string& uri,
					const std::string& host,
					const std::string body,
					const int& contentLength);

		HttpRequest::METHOD			getMethod() const;
		const std::string			getVersion() const;
		const std::string			getUri() const;
		const std::string			getHost() const;
		const std::string			getBody() const;
		const int&					getContentLength() const;
		std::string					translateMethod(HttpRequest::METHOD method) const;

		void setParameters(std::string& uri);
		const std::map<std::string, std::string> getParameters();

	private:
		static const std::string	_allowedMethods[];

		HttpRequest::METHOD	_method;
		const std::string	_version;
		const std::string	_uri;
		const std::string	_host;
		const std::string	_body;
		const int			_contentLength;
		std::map<std::string, std::string> _parameters;

		HttpRequest(void);
};
