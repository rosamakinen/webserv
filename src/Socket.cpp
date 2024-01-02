
#include "Socket.hpp"
#include <cerrno>

Socket::Socket(void)
{
}

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

void Socket::closeConnection(int& connection) const
{
	isCallValid(close(connection), "Failed to close connection", -1, false);
}

void Socket::closeConnections(pollfd *pollfd, int size) const
{
	for (int i = 0; i < size; i++)
	{
		if (pollfd[i].fd == this->_fd)
			continue;
		if (pollfd[i].fd > 0)
			closeConnection(pollfd[i].fd);
	}
}

const std::string Socket::readRequest(int connection, unsigned int buffer_size) const
{
	char buffer[buffer_size];
	std::string input;

	while (1)
	{
		int readBytes = read(connection, buffer, buffer_size);
		if (readBytes < 0)
			throw BadRequestException("Could not read the client");
		else if (readBytes == 0)
			break ;
		buffer[readBytes] = '\0';
		input.append(buffer);
	}

	return input;
}

void Socket::writeResponse(int connection, const std::string response) const
{
	int result = write(connection, response.c_str(), response.size());
	isCallValid(result, "Failed to send response", -1, true);
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
