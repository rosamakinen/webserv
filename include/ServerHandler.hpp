#pragma once

#include "Client.hpp"
#include "Socket.hpp"
#include "Server.hpp"
#include "HttpRequestHandler.hpp"
#include "HttpRequestParser.hpp"
#include "HttpResponseParser.hpp"

#include <sys/poll.h>
#include <functional>

class ServerHandler
{
	private:
		std::vector<pollfd> _pollfds;
		std::map<int, Client*> _clients;

		void	initServers(std::vector<Server*>& servers);

		void	isCallValid(const int fd, const std::string errorMsg, int closeFd);

		bool	incomingClient(int fd, std::vector<Server*>& servers);
		void	handleNewClient(Socket *socket);
		void	handlePollEvents(std::vector<Server*>& servers);

		void	handleIncomingRequest(pollfd *fd);
		void	handleOutgoingResponse(pollfd *fd);
		void	handleOutgoingError(const Exception& e, pollfd *fd);

		void	closeConnections();
		void	closeConnection(int fd);

		std::string	readRequest(int connection, unsigned int buffer_size);
		void		writeResponse(int connection, const std::string response);

	public:
		ServerHandler();
		~ServerHandler();

		void	addNewPoll(int fd);
		void	runServers(std::vector<Server*>& servers);
};
