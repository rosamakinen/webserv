
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

		HttpResponse(void);
		HttpResponse(const HttpResponse& rhs);
		HttpResponse&	operator=(const HttpResponse& rhs);

	public:
		~HttpResponse(void);
		HttpResponse(const std::pair<unsigned int, std::string> &status);

		void						setBody(const std::string body);
		void						setStatus(const std::pair<unsigned int, std::string> &status);

		unsigned int										getContentLenght() const;
		const std::string									getContentType() const;
		const std::string									getBody() const;
		const std::pair<unsigned int, std::string>			getStatus() const;
};

std::ostream& operator<<(std::ostream &out, const HttpResponse &rhs);
