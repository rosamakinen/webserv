#pragma once

#include <sys/socket.h> // For socket functions
#include <netinet/in.h> // For sockaddr_in
#include <iostream>
#include <fcntl.h>
#include <unistd.h> // For read

class Socket
{
	private:
		int			fd;
		sockaddr_in	address;

		Socket(void);

		static void	isCallValid(const int fd, const std::string errorMsg, int closeFd);

	public:
		Socket(int portNumber);
		~Socket(void);
		Socket(const Socket& rhs);

		int					acceptConnection();
		const std::string	readRequest(int connection, unsigned int buffer_size) const;
		void				writeResponse(int connection, const std::string response) const;

		int					getFd() const;

		Socket& operator=(const Socket& rhs);
};
