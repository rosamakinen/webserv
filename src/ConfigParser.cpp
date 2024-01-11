#include "../include/ConfigParser.hpp"

void processLine(const std::string& line)
{
	std::istringstream iss(line);
	std::string keyword;
	iss >> keyword;

	if (keyword == "server") 
	{
		// Process server information
		std::cout << "Found server block" << std::endl;
	} 
	else if (keyword == "main") 
	{
		// Process main information
		std::cout << "Found main block" << std::endl;
	} 
	else if (keyword == "location") 
	{
		std::string locationName;
		iss >> locationName;
		// Process location information
		std::cout << "Found location block: " << locationName << std::endl;
	} 
	else 
	{
		std::cout << "Unknown keyword: " << keyword << std::endl;
	}
}

static std::string trim(const std::string& str)
{
		size_t first = str.find_first_not_of(' ');
		if (first == std::string::npos) 
			return "";
		size_t last = str.find_last_not_of(' ');
		return str.substr(first, (last - first + 1));
}

void parseConfig(const std::string& filename) 
{
	std::ifstream file(filename);
	std::string line;

	if (!file.is_open()) {
		std::cerr << "Failed to open file: " << filename << std::endl;
		return;
	}

	while (std::getline(file, line)) 
	{
		// Ignore comments
		size_t commentPos = line.find('#');
		if (commentPos != std::string::npos)
			line = line.substr(0, commentPos);

		// Trim whitespace
		line = trim(line);

		if (!line.empty()) {
			processLine(line);
		}
	}
}