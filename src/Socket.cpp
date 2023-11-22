#include "Socket.hpp"

Socket::Socket(void)
{
}

void	Socket::isCallValid(const int fd, const std::string errorMsg)
{
	if (fd < 0)
		std::cerr << errorMsg << std::endl;
}

Socket::Socket(const int portNumber) : fd(-1), connection(-1)
{
	this->fd = socket(AF_INET, SOCK_STREAM, 0);
	isCallValid(this->fd, "Failed to create the socket");

	this->address.sin_family = AF_INET;
	this->address.sin_addr.s_addr = INADDR_ANY;
	this->address.sin_port = htons(portNumber);

	int	result = bind(this->fd, (struct sockaddr*)&this->address, sizeof(this->address));
	isCallValid(result, "Failed to bind to port");

	result = listen(this->fd, 10);
	isCallValid(result, "Failed to listen on socket");
}

Socket::~Socket(void)
{
	close(this->connection);
	close(this->fd);
}

Socket::Socket(const Socket &rhs) : fd(rhs.fd), connection(rhs.connection)
{
}

void Socket::acceptConnection()
{
	size_t socketSize = sizeof(this->address);
	this->connection = accept(this->fd, (struct sockaddr*)&this->address, (socklen_t*)&socketSize);
	isCallValid(this->connection, "Failed to accept connection");
}

const std::string Socket::readRequest() const
{
	char buffer[100];
	int result = read(this->connection, buffer, 100);
	std::cout << result << std::endl;
	buffer[result] = '\0';
	isCallValid(result, "Failed to read request");
	std::string input(buffer);
	return input;
}

void Socket::writeResponse(const std::string response) const
{
	int result = send(connection, response.c_str(), response.size(), 0);
	isCallValid(result, "Failed to send response");
}

Socket &Socket::operator=(const Socket &rhs)
{
	if (this != &rhs)
	{
		this->fd = rhs.fd;
		this->connection = rhs.connection;
	}

	return *this;
}
