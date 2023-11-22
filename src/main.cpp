#include <cstdlib> // For exit() and EXIT_FAILURE
#include <iostream> // For cout
#include <unistd.h> // For read
#include "../include/Socket.hpp"

int main()
{
	Socket socket(9999);

	socket.acceptConnection();
	while (1)
	{
		std::string request = socket.readRequest();
		std::cout << "The message was: " << request;
		if (!request.compare("Q\r\n"))
			break ;

		std::string response = "This is the response\n";
		socket.writeResponse(response);
	}

	return 0;
}
