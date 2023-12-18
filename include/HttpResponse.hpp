
#pragma once

#include <iostream>

#include "../include/WebServer.hpp"

class HttpResponse
{
	private:
		HttpResponse(void);
		HttpResponse&	operator=(const HttpResponse& rhs);

	public:
		static const std::string	_version;

		const std::string	_date;
		const std::string	_contentType;
		unsigned int		_contentLenght;
		httpStatus			_status;

		std::string	_body;


		~HttpResponse(void);
		HttpResponse(const HttpResponse& rhs);
		HttpResponse(const std::string date,
					const std::string contentType);

		void						setBody(const std::string body);
		void						setStatus(const unsigned int code, const std::string message);

		const std::string			getDate() const;
		unsigned int				getContentLenght() const;
		const std::string			getContentType() const;
		const std::string			getBody() const;
};

std::ostream& operator<<(std::ostream &out, const HttpResponse &rhs);
