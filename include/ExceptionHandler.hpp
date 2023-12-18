
#pragma once

#include "WebServer.hpp"
#include "HttpResponse.hpp"
#include "Exceptions.hpp"

#include <map>
#include <typeinfo>

class ExceptionHandler
{
	public:
		static httpStatus *getErrorStatus(const Exception& e);
};
