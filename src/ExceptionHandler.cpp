
#include "../include/ExceptionHandler.hpp"

std::pair<unsigned int, std::string> ExceptionHandler::getErrorStatus(const Exception& e)
{
	// If we fall all the way here, let's just give a 500
	std::pair<unsigned int, std::string> status = std::pair<unsigned int, std::string>(500, "Internal Server Error");

	if (typeid(e) == typeid(InternalException))
	{
		status = std::pair<unsigned int, std::string>(500, "Internal Server Error");
	}

	if (e._showToUser)
	{
		std::cerr << e.what() << std::endl;
	}

	return status;
}
