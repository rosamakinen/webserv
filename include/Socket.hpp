#ifndef SOCKET_HPP
#define SOCKET_HPP

#include "WebServer.hpp"
#include "Exceptions.hpp"

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstring>

class Socket
{
	private:
		Socket(void);
		int _fd;
		std::string _host;
		int _port;
		sockaddr_in	_address;


		static void			isCallValid(const int fd, const std::string errorMsg, int closeFd, bool keepRunning);

	public:
		Socket(const int portNumber, std::string host);
		~Socket(void);
		Socket(const Socket& rhs);

		int	acceptConnection() const;
		int	getFd() const;

		Socket& operator=(const Socket& rhs);
};

#endif
