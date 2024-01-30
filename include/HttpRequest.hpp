
#pragma once

#include "Util.hpp"
#include "WebServer.hpp"
#include "Exceptions.hpp"

class HttpRequest
{
	public:
		~HttpRequest(void);
		HttpRequest(Util::METHOD method,
					const std::string& version,
					const std::string& uri,
					const std::string& host,
					const std::string body,
					const int& contentLength);

		Util::METHOD		getMethod() const;
		const std::string	getVersion() const;
		const std::string	getUri() const;
		const std::string	getHost() const;
		const std::string	getBody() const;
		const int&			getContentLength() const;

		void setParameters(std::map<std::string, std::string> parameters);
		const std::map<std::string, std::string> getParameters();

	private:
		static const std::string	_allowedMethods[];

				Util::METHOD		_method;
		const std::string			_version;
		const std::string			_uri;
		const std::string			_host;
		const std::string			_body;
		const int					_contentLength;
		std::map<std::string, std::string> _parameters;

		HttpRequest(void);
};
