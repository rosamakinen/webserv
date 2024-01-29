
#include "Socket.hpp"
#include <cerrno>

void	Socket::isCallValid(const int fd, const std::string errorMsg, int closeFd, bool keepRunning)
{
	if (fd < 0)
	{
		if (closeFd != -1)
			close(closeFd);
		if (!keepRunning)
			throw ConfigurationException(errorMsg);
		else
			throw InternalException(errorMsg);
	}
}

Socket::Socket(const int portNumber) : _fd(-1)
{
	this->_fd = socket(AF_INET, SOCK_STREAM, 0);
	isCallValid(this->_fd, "Failed to create the socket", -1, false);

	// Make port and address reusable for multple sockets
	int opt = 1;
	int result = setsockopt(this->_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
	isCallValid(result, "Failed to set SO_REUSEADDR option", this->_fd, false);
	result = setsockopt(this->_fd, SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(opt));
	isCallValid(result, "Failed to set SO_REUSEPORT option", this->_fd, false);

	// Make socket non-blocking by adding flag
	result = fcntl(this->_fd, F_SETFL, fcntl(this->_fd, F_GETFL, 0) | O_NONBLOCK, FD_CLOEXEC);
	isCallValid(result, "Failed to set socket as non/blocking", this->_fd, false);

	this->_address.sin_family = AF_INET;
	this->_address.sin_addr.s_addr = htonl(INADDR_ANY);
	this->_address.sin_port = htons(portNumber);

	result = bind(this->_fd, (struct sockaddr*)&this->_address, sizeof(this->_address));
	isCallValid(result, "Failed to bind to port", this->_fd, false);

	result = listen(this->_fd, 10);
	isCallValid(result, "Failed to listen on socket", this->_fd, false);
}

Socket::~Socket(void)
{
	isCallValid(close(this->_fd), "Failed to close socket", -1, false);
}

Socket::Socket(const Socket &rhs) : _fd(rhs._fd)
{
}

int Socket::acceptConnection() const
{
	size_t socketSize = sizeof(this->_address);
	int connection = accept(this->_fd, (struct sockaddr*)&this->_address, (socklen_t*)&socketSize);

	return connection;
}

int Socket::getFd() const
{
	return this->_fd;
}

Socket &Socket::operator=(const Socket &rhs)
{
	if (this != &rhs)
		this->_fd = rhs._fd;

	return *this;
}
