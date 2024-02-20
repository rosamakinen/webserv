#include "../include/WebServer.hpp"
#include "../include/Socket.hpp"

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

Socket::Socket(const int portNumber, std::string host) : _fd(-1)
{
	this->_port = portNumber;
	this->_fd = socket(PF_INET, SOCK_STREAM, 0);
	isCallValid(this->_fd, "Failed to create the socket", -1, false);

	// Make socket non-blocking by adding flag
	int result = fcntl(this->_fd, F_SETFL, O_NONBLOCK, FD_CLOEXEC);
	isCallValid(result, "Failed to set socket as non/blocking", this->_fd, false);

	this->_address.sin_family = AF_INET;
	this->_address.sin_port = htons(this->_port);
	this->_address.sin_addr.s_addr = inet_addr(host.c_str());
	memset(this->_address.sin_zero, '\0', sizeof(this->_address.sin_zero));

	int opt = 1;
	setsockopt(this->_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(int));

	result = bind(this->_fd, (struct sockaddr*)&this->_address, sizeof(this->_address));
	isCallValid(result, "Failed to bind to port", this->_fd, false);

	result = listen(this->_fd, 20);
	isCallValid(result, "Failed to listen on socket", this->_fd, false);
}

Socket::~Socket(void)
{
	isCallValid(close(this->_fd), "Failed to close socket", -1, false);
}

Socket::Socket(const Socket &rhs) : _fd(rhs._fd)
{
}

int Socket::acceptConnection(int fd) const
{
	sockaddr_in client_address;
	socklen_t	client_address_size = sizeof(sockaddr_in);
	int connection = accept(fd, (struct sockaddr*)&client_address, &client_address_size);
	if (connection == -1)
		return -1;

	// Make non-blocking by adding flag
	int result = fcntl(connection, F_SETFL, O_NONBLOCK, FD_CLOEXEC);
	isCallValid(result, "Failed to set connection as non/blocking", connection, false);

	int bufferSize = MESSAGE_BUFFER;
	result = setsockopt(connection, SOL_SOCKET, SO_SNDBUF, &bufferSize, sizeof(bufferSize));
	isCallValid(result, "Failed to set buffer size option", connection, false);

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
