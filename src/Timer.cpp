
#include "../include/WebServer.hpp"
#include "../include/Timer.hpp"

const std::string Timer::GetTimeDateString()
{
	// Initialize timezone information
	tzset();
	// Get local machine time and convert to UTC time
	time_t now = time(0);
	struct tm tm = *gmtime(&now);

	// Stringify the struct to a string representation
	char date[1000];
	strftime(date, sizeof(date), "%a, %d %b %Y %T %Z", &tm);
	return (std::string(date));
}
