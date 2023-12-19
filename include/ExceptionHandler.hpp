
#include "WebServer.hpp"
#include "HttpResponse.hpp"
#include "Exceptions.hpp"

#include <map>
#include <typeinfo>

class ExceptionHandler
{
	public:
		static std::pair<unsigned int, std::string> getErrorStatus(const Exception& e);
};
