#include "../include/FileHandler.hpp"
#include "../include/WebServer.hpp"
#include "FileHandler.hpp"

static std::string getDirectoryFromUri(const std::string& uri)
{
	size_t pos = uri.find_last_of('/');
	return (pos == std::string::npos) ? "" : uri.substr(0, pos + 1);
}

static std::string getFileFromUri(const std::string& uri)
{
	size_t pos = uri.find_last_of('/');
	return (pos == std::string::npos) ? uri : uri.substr(pos + 1);
}

std::string FileHandler::getFilePath(std::string relativePath)
{
	char file_path[MESSAGE_BUFFER];
	getcwd(file_path, MESSAGE_BUFFER);

	std::cout << "Absolute Path: " << std::string(file_path).append(relativePath) << std::endl;
	return std::string(file_path).append(relativePath);
}

std::string FileHandler::buildDirListing(std::string full_path)
{
	DIR *dir_path = opendir(full_path.c_str());
	if (!dir_path)
		throw NotFoundException("Could not create directory listing");

	std::string html;
	html.append("<html>\n");
	html.append("<head>\n");
	html.append("<title>Listing of ");
	html.append(full_path.c_str());
	html.append("</title>\n");
	html.append("</head>\n");
	html.append("<body>\n");
	html.append("<h1>Listing of ");
	html.append(full_path);
	html.append("</h1>\n");
	html.append("<table style=\"width:80%; font_size: 15px\">\n");
	html.append("<hr>\n");
	html.append("<th style=\"text-align: left\"> File name </th>\n");
	html.append("<th style=\"text-align: left\"> Last modified </th>\n");
	html.append("<th style=\"text-align: left\"> File size </th>\n");

	struct stat file_status;
	struct dirent *directory;
	while ((directory = readdir(dir_path)))
	{
		if (strcmp(directory->d_name, ".") == 0)
			continue;
		std::string file_path = full_path + directory->d_name;
		stat(file_path.c_str(), &file_status);
		html.append("<tr>\n");
		html.append("<td>\n");
		html.append("<a href=\"");
		html.append(directory->d_name);
		if (S_ISDIR(file_status.st_mode))
			html.append("/");
		html.append("\">");
		html.append(directory->d_name);
		if (S_ISDIR(file_status.st_mode))
			html.append("/");
		html.append("</a>\n");
		html.append("</td>\n");
		html.append("<td>\n");
		html.append(ctime(&file_status.st_mtime));
		html.append("</td>\n");
		html.append("<td>\n");
		if (!S_ISDIR(file_status.st_mode))
			html.append(std::to_string(file_status.st_size));
		html.append("</td>\n");
		html.append("</tr>\n");
	}

	html.append("</table>\n");
	html.append("<hr>\n");
	html.append("</body>\n");
	html.append("</html>\n");

	closedir(dir_path);

	return html;
}

static bool isAutoIndexAllowed(std::string path, Server *server)
{
	const std::vector<std::string> *values = server->getLocationValue(path, AUTO_INDEX);
	if (values != nullptr && values->size() >= 1 && values->front().compare("true") == 0)
		return true;
	throw ForbiddenException("Directory listing not allowed for this location");
}

std::string FileHandler::getFileResource(std::string path, std::ios_base::openmode mode, Server *server)
{
	std::ifstream file;
	std::string workingPath = getDirectoryFromUri(path), workingFile = getFileFromUri(path);

	const std::vector<std::string>* workingDir = server->getLocationValue(workingPath, LOCAL_DIR);
	if (workingDir != nullptr && workingDir->size() == 1)
		workingPath = workingDir->at(0);
	else
		throw BadRequestException("Directory key has missing or invalid values.");
	path = workingPath.append(workingFile);
	
	std::string full_path = getFilePath(path);
	if (full_path[full_path.length() - 1] == '/' && isAutoIndexAllowed(workingPath, server))
		return buildDirListing(full_path);

	file.open(full_path, mode);
	if (!file.is_open() || file.fail() || file.bad())
	{
		std::string message = "Could not open file ";
		message.append(full_path);
		message.append(" for reading");
		throw NotFoundException(message);
	}

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
	if (!file.is_open() || file.fail() || file.bad())
	{
		std::string message = "Could not open file ";
		message.append(path);
		message.append(" for reading");
		throw FileException(message);
	}

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
	if (!file.is_open() || file.fail() || file.bad())
	{
		std::string message = "Could not open file ";
		message.append(path);
		message.append(" for reading");
		throw FileException(message);
	}

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
