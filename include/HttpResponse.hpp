
#pragma once

#include <iostream>

class HttpResponse
{
	private:
	public:
		static const std::string	_version;

		const std::string	_date;
		const unsigned int	_contentLenght;
		const std::string	_contentType;
		std::pair<unsigned int, std::string>	_status;

		std::string	_body;

		HttpResponse(void);

		~HttpResponse(void);
		HttpResponse(const HttpResponse& rhs);
		HttpResponse(const std::string date,
					const unsigned int contentLenght,
					const std::string contentType);

		HttpResponse&	operator=(const HttpResponse& rhs);

		void						setBody(const std::string body);
		void						setStatus(const std::pair<unsigned int, std::string> status);

		const std::string			getDate() const;
		unsigned int				getContentLenght() const;
		const std::string			getContentType() const;
		const std::string			getBody() const;
};

std::ostream& operator<<(std::ostream &out, const HttpResponse &rhs);
