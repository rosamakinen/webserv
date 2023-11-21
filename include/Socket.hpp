#pragma once

#include <sys/socket.h> // For socket functions
#include <netinet/in.h> // For sockaddr_in
#include <iostream>
#include <unistd.h> // For read

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

		void		acceptConnection();
		const std::string	readRequest() const;
		void		writeResponse(const std::string response) const;

		Socket& operator=(const Socket& rhs);
};
