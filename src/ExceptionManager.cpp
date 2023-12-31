
#include "../include/ExceptionManager.hpp"

std::pair<unsigned int, std::string> ExceptionManager::getErrorStatus(const Exception& e)
{
	// Log the exception
	std::cerr << e.what() << std::endl;

	if (typeid(e) == typeid(InternalException))
		return std::pair<unsigned int, std::string>(500, "Internal Server Error");
	else if (typeid(e) == typeid(BadRequestException))
		return std::pair<unsigned int, std::string>(400, "Bad Request");

	// If we fall all the way down here, let's just give a 500
	return std::pair<unsigned int, std::string>(500, "Internal Server Error");
}
