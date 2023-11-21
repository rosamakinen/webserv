#pragma once

#include <sys/socket.h> // For socket functions
#include <netinet/in.h> // For sockaddr_in
#include <iostream>

class Socket
{
	private:
		int			fd;
		int			connection;
		sockaddr_in	address;

		Socket(void);

		static void	isCallValid(const int fd, const std::string errorMsg);

	public:
		Socket(int portNumber);
		~Socket(void);
		Socket(const Socket& rhs);

		Socket& operator=(const Socket& rhs);
};
