#include "../include/FileHandler.hpp"
#include "../include/WebServer.hpp"

std::string FileHandler::getFileContent(std::string path, bool addCrLf)
{
	char file_path[MESSAGE_BUFFER];
	getcwd(file_path, MESSAGE_BUFFER);
	std::ifstream file(std::string(file_path).append(path));

	if (!file.is_open())
	{
		throw FileException("Could not open file for reading");
	}

	std::string line;
	std::string body;
	while (getline(file, line))
	{
		body.append(line);
		if (addCrLf)
			body.append(HTTP_LINEBREAK);
	}

	file.close();
	return body;
}
