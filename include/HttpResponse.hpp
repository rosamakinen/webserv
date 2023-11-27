
#pragma once

#include <iostream>

class HttpResponse
{
	private:
		static const std::string	version;

		const std::string			date;
		const std::string			serverName;
		const unsigned int			contentLenght;
		const std::string			contentType;
		const unsigned int			statusCode;

		// TODO Add error status / status code handler!
		std::string					status;
		std::string					body;

		HttpResponse(void);

	public:
		~HttpResponse(void);
		HttpResponse(const HttpResponse& rhs);
		HttpResponse(const std::string date,
					const std::string serverName,
					const unsigned int contentLenght,
					const std::string contentType,
					const unsigned int statusCode);

		HttpResponse&	operator=(const HttpResponse& rhs);

		void						setBody(const std::string body);

		const std::string			getDate() const;
		const std::string			getServerName() const;
		unsigned int				getContentLenght() const;
		const std::string			getContentType() const;
		const std::string			getBody() const;
		unsigned int				getStatusCode() const;
};
