
#include "Socket.hpp"
#include <cerrno>

Socket::Socket(void)
{
}

void	Socket::isCallValid(const int fd, const std::string errorMsg, int closeFd)
{
	if (fd < 0)
	{
		std::cerr << errorMsg << std::endl;
		if (closeFd != -1)
			close(closeFd);
	}
}

Socket::Socket(const int portNumber) : fd(-1)
{
	this->fd = socket(AF_INET, SOCK_STREAM, 0);
	isCallValid(this->fd, "Failed to create the socket", -1);

	// Make port and address reusable for multple sockets
	int opt = 1;
	int result = setsockopt(this->fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
	isCallValid(result, "Failed to set SO_REUSEADDR option", this->fd);
	result = setsockopt(this->fd, SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(opt));
	isCallValid(result, "Failed to set SO_REUSEPORT option", this->fd);

	// Make socket non-blocking by adding flag
	result = fcntl(this->fd, F_SETFL, fcntl(this->fd, F_GETFL, 0) | O_NONBLOCK, FD_CLOEXEC);
	isCallValid(result, "Failed to set socket as non/blocking", this->fd);

	this->address.sin_family = AF_INET;
	this->address.sin_addr.s_addr = htonl(INADDR_ANY);
	this->address.sin_port = htons(portNumber);

	result = bind(this->fd, (struct sockaddr*)&this->address, sizeof(this->address));
	isCallValid(result, "Failed to bind to port", this->fd);

	result = listen(this->fd, 10);
	isCallValid(result, "Failed to listen on socket", this->fd);
}

Socket::~Socket(void)
{
	isCallValid(close(this->fd), "Failed to close socket", -1);
}

Socket::Socket(const Socket &rhs) : fd(rhs.fd)
{
}

int Socket::acceptConnection() const
{
	size_t socketSize = sizeof(this->address);
	int connection = accept(this->fd, (struct sockaddr*)&this->address, (socklen_t*)&socketSize);

	return connection;
}

void Socket::closeConnection(int& connection) const
{
	isCallValid(close(connection), "Failed to close connection", -1);
}

void Socket::closeConnections(pollfd *pollfd, int size) const
{
	for (int i = 0; i < size; i++)
	{
		if (pollfd[i].fd == this->fd)
			continue;
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
		if (readBytes <= 0)
			break ;
		buffer[readBytes] = '\0';
		input.append(buffer);
	}

	return input;
}

void Socket::writeResponse(int connection, const std::string response) const
{
	int result = write(connection, response.c_str(), response.size());
	isCallValid(result, "Failed to send response", -1);
}

int Socket::getFd() const
{
	return this->fd;
}

Socket &Socket::operator=(const Socket &rhs)
{
	if (this != &rhs)
		this->fd = rhs.fd;

	return *this;
}
