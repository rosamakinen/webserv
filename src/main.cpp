#include <cstdlib> // For exit() and EXIT_FAILURE
#include <iostream> // For cout
#include <unistd.h> // For read
#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>

#include "../include/Socket.hpp"
#include "../include/Server.hpp"

Server* initServer()
{
	Server *server = new Server("127.0.0.1", 8000);
	server->setName("localhost");
	server->setRequestMaxBodySize(1000);
	std::vector<std::string> methods;
	methods.push_back("GET");
	methods.push_back("POST");
	server->addLocation("/", methods);

	return server;
}

int main()
{
	Server *server = initServer();
	fd_set current_sockets, ready_sockets;
	FD_ZERO(&current_sockets);

	Socket socket(server->getPort());
	int biggest_fd = socket.getFd();
	FD_SET(biggest_fd, &current_sockets);

	struct timeval timer;
	while (1)
	{
		timer.tv_sec = 1;
		timer.tv_usec = 0;

		// Select incoming connections
		ready_sockets = current_sockets;
		int selected = select(FD_SETSIZE, &ready_sockets, NULL, NULL, &timer);
		if (selected < 0)
			break ; // TODO: add error / exit

		for (int i = 0; i < FD_SETSIZE; i++)
		{
			if (FD_ISSET(i, &ready_sockets))
			{
				if (i == socket.getFd())
				{
					int client = socket.acceptConnection();
					FD_SET(client, &current_sockets);
				}
				else
				{
					std::string request = socket.readRequest(i, server->getRequestMaxBodySize());
					std::cout << "The message was: " << request;
					std::cout << request.substr(request.length() - 6, request.length()) << std::endl;
					if (!request.compare("\r\n\r\n"))
						break ;
					std::string response = "This is the response\n";
					socket.writeResponse(i, response);
				}
			}
		}
	}

	delete server;

	return 0;
}
