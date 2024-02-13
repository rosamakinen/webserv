#ifndef CLIENT_HPP
#define CLIENT_HPP
#include "HttpResponse.hpp"
#include "HttpRequest.hpp"

class Client
{
	public:
		Client();
		~Client();

		enum STATUS
		{
			NONE,
			INCOMING,
			READY_TO_HANDLE,
			OUTGOING
		};

		void setResponse(HttpResponse *response);
		void setRequest(HttpRequest *request);

		HttpResponse *getResponse();
		HttpRequest *getRequest();

		STATUS getStatus();
		void setStatus(STATUS status);

	private:
		HttpResponse *_response;
		HttpRequest *_request;

		STATUS _status;
};
#endif
