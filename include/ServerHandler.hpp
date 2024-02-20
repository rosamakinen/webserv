#ifndef SERVERHANDLER_HPP
#define SERVERHANDLER_HPP

#include "WebServer.hpp"
#include "Client.hpp"
#include "Socket.hpp"
#include "Server.hpp"
#include "HttpRequestHandler.hpp"
#include "HttpRequestParser.hpp"
#include "HttpResponseParser.hpp"

class ServerHandler
{
	private:
		std::vector<pollfd> _pollfds;
		std::map<int, Client*> _clients;
		std::map<std::string, Server *> _servers;

		void 	initServers(std::map<std::string, Server*> &servers);

		void	isCallValid(const int fd, const std::string errorMsg, int closeFd);

		Client	*getOrCreateClient(pollfd *fd);
		void	handleReadyToBeHandledClients();
		bool 	incomingClient(int fd);
		void	handleNewClient(Socket *socket);
		void 	handlePollEvents();

		void	removeTimedOutClients();
		std::map<int, Client*>::iterator removeClient(std::map<int, Client*>::iterator client);
		bool	hasTimedOut(Client *client);

		void	handleIncomingRequest(pollfd *fd);
		void	handleOutgoingResponse(pollfd *fd);
		void	handleOutgoingError(const Exception& e, pollfd *fd);

		void	closeConnections();
		void	closeConnection(int fd);

		std::string	readRequest(int connection, unsigned int buffer_size);
		void	writeResponse(int connection, const std::string response);

		Server	*getServer(HttpRequest *request);

	public:
		ServerHandler();
		~ServerHandler();

		void	addNewPoll(int fd);
		void	runServers(std::map<std::string, Server*> &servers);
};

#endif
