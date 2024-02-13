#ifndef EXCEPTIONMANAGER_HPP
#define EXCEPTIONMANAGER_HPP

#include "WebServer.hpp"
#include "Exceptions.hpp"

class ExceptionManager
{
	public:
		static std::pair<unsigned int, std::string> getErrorStatus(const Exception& e);
};

#endif
