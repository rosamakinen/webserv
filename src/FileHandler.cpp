#include "../include/FileHandler.hpp"
#include "../include/WebServer.hpp"

std::string FileHandler::getFilePath(std::string relativePath)
{
	char file_path[MESSAGE_BUFFER];
	getcwd(file_path, MESSAGE_BUFFER);

	return std::string(file_path).append(relativePath);
}

std::string FileHandler::getFileResource(std::string path, std::ios_base::openmode mode)
{
	std::ifstream file;
	std::string full_path = getFilePath(path);
	if (full_path[full_path.length() - 1] == '/')
		full_path.append("index.html");

	file.open(full_path, mode);
	if (!file.is_open() || file.fail() || file.bad())
		throw NotFoundException("Could not open file for reading");

	std::string body;
	if (mode == std::ios::binary)
	{
		std::stringstream contents;
		contents << file.rdbuf();
		body.append(contents.str());
	}
	else
	{
		std::string line;
		while (getline(file, line))
		{
			body.append(line);
			body.append(HTTP_LINEBREAK);
		}
	}

	file.close();
	return body;
}

std::string FileHandler::getFileContent(std::string path)
{
	std::ifstream file(getFilePath(path));
	if (!file.is_open())
		throw NotFoundException("Could not open file for reading");

	std::string line;
	std::string body;
	while (getline(file, line))
		body.append(line);

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
