#ifndef CLIENT_HPP
#define CLIENT_HPP

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
		void appendRequest(std::string requestToAdd);
		void setServer(Server *server);
		void updateStatus();
		bool closeConnection();
		void setCloseConnection(bool close);

		HttpResponse *getResponse();
		HttpRequest *getRequest();
		Server *getServer();

		STATUS getStatus();
		void setStatus(STATUS status);

		std::chrono::high_resolution_clock::time_point getRequestStart();

	private:
		HttpResponse *_response;
		HttpRequest *_request;
		Server *_server;
		STATUS _status;
		bool _closeConnection;

		std::chrono::high_resolution_clock::time_point _requestStart;
};
#endif
