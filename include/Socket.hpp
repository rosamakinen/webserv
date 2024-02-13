#ifndef SOCKET_HPP
#define SOCKET_HPP

#include "WebServer.hpp"
#include "Exceptions.hpp"

class Socket
{
	private:
		Socket(void);
		int _fd;
		sockaddr_in	_address;


		static void			isCallValid(const int fd, const std::string errorMsg, int closeFd, bool keepRunning);

	public:
		Socket(int portNumber);
		~Socket(void);
		Socket(const Socket& rhs);

		int	acceptConnection() const;
		int	getFd() const;

		Socket& operator=(const Socket& rhs);
};

#endif
