
#include "../include/ExceptionHandler.hpp"

httpStatus *ExceptionHandler::getErrorStatus(const Exception& e)
{
	// Log the exception
	std::cerr << e.what() << std::endl;

	// If we fall all the way here, let's just give a 500
	struct httpStatus *status = new httpStatus;
	status->code = 500;
	status->message = "Internal Server Error";

	if (typeid(e) == typeid(InternalException))
		return status;

	return status;
}
