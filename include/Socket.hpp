#pragma once

#include <sys/socket.h> // For socket functions
#include <netinet/in.h> // For sockaddr_in
#include <iostream>
#include <unistd.h> // For read
#include <poll.h>
#include <fcntl.h>

#include "../include/Exceptions.hpp"

/* TODO: handle these errors in poll [EAGAIN],[EINTR],[EINVAL]*/

class Socket
{
	private:
		int			_fd;
		sockaddr_in	_address;

		Socket(void);

		static void			isCallValid(const int fd, const std::string errorMsg, int closeFd);
		void				closeConnection(int& connection) const;

	public:
		class ConfigurationException : public std::exception
		{
			public:
				const char* what() const throw();
		};

		Socket(int portNumber);
		~Socket(void);
		Socket(const Socket& rhs);

		int					acceptConnection() const;
		void				closeConnections(pollfd *pollfd, int size) const;
		const std::string	readRequest(int connection, unsigned int buffer_size) const;
		void				writeResponse(int connection, const std::string response) const;

		int					getFd() const;

		Socket& operator=(const Socket& rhs);
};
