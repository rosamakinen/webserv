#include "../include/ConfigParser.hpp"

void ConfigParser::parseConfig(const std::string& filename) 
{
	std::ifstream file(filename);
	std::string line;

	if (!file.is_open()) 
	{
		std::cerr << "Failed to open file: " << filename << std::endl;
		return;
	}

	std::string currentSection;
	while (std::getline(file, line)) 
	{
		// Ignore comments
		size_t commentPos = line.find('#');
		if (commentPos != std::string::npos) 
		{
			line = line.substr(0, commentPos);
		}

		// Trim whitespace
		line = trim(line);

		if (!line.empty()) 
		{
			if (line == "{") 
			{
				sectionStack.push_back(currentSection);
			} 
			else if (line == "}") 
			{
				if (!sectionStack.empty()) 
				{
					sectionStack.pop_back();
					if (!sectionStack.empty()) 
					{
						currentSection = sectionStack.back();
					} 
					else 
					{
						currentSection.clear();
					}
				}
			} 
			else 
			{
				processLine(line);
			}
		}
	}
}

void ConfigParser::processLine(const std::string& line) 
{
	std::istringstream iss(line);
	std::string keyword;
	iss >> keyword;

	if (keyword == "server" || keyword == "main" || keyword == "location") {
		currentSection = keyword;
		if (keyword == "location") 
		{
			std::string locationName;
			iss >> locationName;
			currentSection += " " + locationName;
		}
	} 
	else 
	{
		std::cout << "Processing '" << line << "' in section '" << currentSection << "'" << std::endl;
	}
}

std::string ConfigParser::trim(const std::string& str) 
{
	size_t first = str.find_first_not_of(' ');
	if (first == std::string::npos) 
		return "";
	size_t last = str.find_last_not_of(' ');
	return str.substr(first, (last - first + 1));
}
