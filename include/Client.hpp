
#pragma once

#include "HttpResponse.hpp"
#include "HttpRequest.hpp"

#include <chrono>

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

		std::chrono::high_resolution_clock::time_point getRequestStart();

	private:
		HttpResponse *_response;
		HttpRequest *_request;

		std::chrono::high_resolution_clock::time_point _requestStart;

		STATUS _status;
};
