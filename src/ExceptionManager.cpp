#include "../include/ExceptionManager.hpp"

std::pair<unsigned int, std::string> ExceptionManager::getErrorStatus(const Exception& e)
{
	// Log the exception
	std::cerr << e.what() << std::endl;

	if (typeid(e) == typeid(InternalException))
		return std::pair<unsigned int, std::string>(500, "Internal Server Error");
	else if (typeid(e) == typeid(BadRequestException))
		return std::pair<unsigned int, std::string>(400, "Bad Request");
	else if (typeid(e) == typeid(MethodNotAllowedException))
		return std::pair<unsigned int, std::string>(405, "Method Not Allowed");
	else if (typeid(e) == typeid(NotFoundException))
		return std::pair<unsigned int, std::string>(404, "Not Found");
	else if (typeid(e) == typeid(ForbiddenException))
		return std::pair<unsigned int, std::string>(403, "Forbidden");
	else if (typeid(e) == typeid(UnsupportedMediaTypeException))
		return std::pair<unsigned int, std::string>(415, "Unsupported Media Type");
	else if (typeid(e) == typeid(PayloadTooLargeException))
		return std::pair<unsigned int, std::string>(413, "Payload Too Large");
	else if (typeid(e) == typeid(GatewayTimeoutException))
		return std::pair<unsigned int, std::string>(504, "Gateway Timeout");
	else if (typeid(e) == typeid(NotImplementedException))
		return std::pair<unsigned int, std::string>(501, "Not Implemented");
	else if (typeid(e) == typeid(ErrorInCGIException))
		return std::pair<unsigned int, std::string>(502, "Error in CGI application");

	// If we fall all the way down here, let's just give a 500
	return std::pair<unsigned int, std::string>(500, "Internal Server Error");
}
