#include <cstdlib> // For exit() and EXIT_FAILURE
#include <iostream> // For cout
#include <unistd.h> // For read
#include "../include/Socket.hpp"
#include "../include/Server.hpp"

int main()
{
	//// Check for a simple socket initialization
	//Socket socket(9999);

	//socket.acceptConnection();
	//while (1)
	//{
	//	std::string request = socket.readRequest();
	//	std::cout << "The message was: " << request;
	//	if (!request.compare("Q\r\n"))
	//		break ;

	//	std::string response = "This is the response\n";
	//	socket.writeResponse(response);
	//}

	// Check to see if server initialization is okay
	//Server *server = new Server("127.0.0.1", 8000);
	//server->setName("localhost");
	//server->setRequestMaxBodySize(1);
	//std::vector<std::string> methods;
	//methods.push_back("GET");
	//methods.push_back("POST");
	//server->addLocation("/", methods);

	//std::cout << server->getIp() << std::endl;
	//std::cout << server->getPort() << std::endl;
	//std::cout << server->getName() << std::endl;
	//std::cout << server->getRequestMaxBodySize() << std::endl;

	delete server;

	return 0;
}
