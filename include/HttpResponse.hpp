
#pragma once

#include "WebServer.hpp"
#include "Exceptions.hpp"

class HttpResponse
{
	private:
		const std::string	_contentType;
		unsigned int		_contentLenght;
		std::pair<unsigned int, std::string> _status;
		std::string	_body;

	public:
		~HttpResponse(void);
		HttpResponse(const std::pair<unsigned int, std::string> &status, const std::string& resourcePath);

		void						setBody(const std::string body);
		void						setStatus(const std::pair<unsigned int, std::string> &status);

		unsigned int										getContentLenght() const;
		const std::string									getContentType() const;
		const std::string									getBody() const;
		const std::pair<unsigned int, std::string>			getStatus() const;
};

std::ostream& operator<<(std::ostream &out, const HttpResponse &rhs);
