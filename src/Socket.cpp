#include "Socket.hpp"

Socket::Socket(void)
{

}

void	Socket::isCallValid(const int fd, const std::string errorMsg)
{
	if (fd < 0)
	{
		std::cerr << errorMsg << std::endl;
	}
}

Socket::Socket(const int portNumber)
{
	this->fd = socket(AF_INET, SOCK_STREAM, 0);
	isCallValid(this->fd, "Failed to create the socket");

	this->address.sin_family = AF_INET;
	this->address.sin_addr.s_addr = INADDR_ANY;
	this->address.sin_port = htons(portNumber);
	int	bindResult = bind(this->fd, (struct sockaddr*)&this->address, sizeof(this->address));
	isCallValid(bindResult, "Failed to create the socket");
}

Socket::~Socket(void)
{
}

Socket::Socket(const Socket &rhs) : fd(rhs.fd), connection(rhs.connection)
{
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
