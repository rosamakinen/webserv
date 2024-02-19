#ifndef HTTPRESPONSE_HPP
#define HTTPRESPONSE_HPP

#include "WebServer.hpp"
#include "Exceptions.hpp"
#include "FileHandler.hpp"
#include "Server.hpp"

class HttpResponse
{
	public:
		static std::map<std::string, std::string> _contentTypes;

		HttpResponse();
		~HttpResponse(void);

		void setBody(const std::string body);
		void setStatus(const std::pair<unsigned int, std::string> &status);
		bool contentTypeSet(std::string resourcePath, std::string contentTypeToFind);
		std::ios_base::openmode setContentType(std::string resourcePath);
		void setCgiResponse(std::string input);
		void setDeleteResponse();
		void setPostResponse();

		unsigned int getContentLenght() const;
		const std::string getContentType() const;
		const std::string getBody() const;
		const std::pair<unsigned int, std::string> getStatus() const;
		const std::string getCgiResponse() const;

		void setResponseBody(HttpRequest *request, Server *server);

	private:
		std::string _contentType;
		unsigned int _contentLenght;
		std::pair<unsigned int, std::string> _status;
		std::string _body;
		std::string _cgiResponse;
};

std::ostream& operator<<(std::ostream &out, const HttpResponse &rhs);

#endif
