#include "../include/FileHandler.hpp"
#include "../include/WebServer.hpp"

static std::string getFilePath(std::string relativePath)
{
	char file_path[MESSAGE_BUFFER];
	getcwd(file_path, MESSAGE_BUFFER);
	return std::string(file_path).append(relativePath);
}

std::string FileHandler::getFileContent(std::string path, bool addCrLf)
{
	std::ifstream file(getFilePath(path));
	if (!file.is_open())
		throw FileException("Could not open file for reading");

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

std::string FileHandler::getErrorFileContent(unsigned int status)
{
	std::string relativePath("/public_www/error_pages/");
	relativePath.append(std::to_string(status)).append(".html");

	std::string path(getFilePath(relativePath));
	std::ifstream file(path);
	if (!file.is_open())
		throw FileException("Could not open file for reading");

	std::string line;
	std::string body;
	while (getline(file, line))
	{
		body.append(line);
		body.append(HTTP_LINEBREAK);
	}

	file.close();
	return body;
}