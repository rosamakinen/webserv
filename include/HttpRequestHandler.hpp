#ifndef HTTPREQUESTHANDLER_HPP
#define HTTPREQUESTHANDLER_HPP

#include "WebServer.hpp"
#include "Exceptions.hpp"
#include "HttpRequest.hpp"
#include "HttpResponse.hpp"
#include "FileHandler.hpp"
#include "ExceptionManager.hpp"
#include "Client.hpp"
#include "CgiHandler.hpp"
#include "Server.hpp"
#include "Methods.hpp"

class HttpRequestHandler
{
	private:
		void parseOkResponse(Client *client, Server *server);

	public:
		HttpRequestHandler();
		~HttpRequestHandler();

		HttpResponse *parseErrorResponse(Server *server, std::pair<unsigned int, std::string> status);
		void handleRequest(Client *client, Server *server);
		bool redirectClient(Client *client);
		void parseRedirResponse(Client *client, Server *server);
};

#endif
