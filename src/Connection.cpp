#include "../include/Connection.hpp"

Connection::Connection()
{
	_lastTrafficTS = std::chrono::high_resolution_clock::now();
}

void Connection::updateTS()
{
	_lastTrafficTS = std::chrono::high_resolution_clock::now();
}

std::chrono::high_resolution_clock::time_point Connection::getTS()
{
	return this->_lastTrafficTS;
}
