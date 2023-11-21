
#pragma once

#include <iostream>

class HttpRequest
{
	private:
		static const std::string	allowedVersion;
		static const std::string	allowedMethods[];

		const std::string			method;
		const std::string			version;
		const std::string			uri;
		const std::string			host;
		const std::string			body;
		const int					contentLenght;

		HttpRequest(void);

		static void					isValid(const HttpRequest& request);

	public:
		~HttpRequest(void);
		HttpRequest(const HttpRequest& rhs);
		HttpRequest(const std::string& method,
					const std::string& version,
					const std::string& uri,
					const std::string& host,
					const std::string body,
					const int& contentLenght);

		HttpRequest&	operator=(const HttpRequest& rhs);

		const std::string			getMethod() const;
		const std::string			getVersion() const;
		const std::string			getUri() const;
		const std::string			getHost() const;
		const std::string			getBody() const;
		const int&					getContentLenght() const;
};
