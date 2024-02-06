#include "../include/FileHandler.hpp"

std::string FileHandler::getFilePath(std::string relativePath)
{
	char file_path[MESSAGE_BUFFER];
	getcwd(file_path, MESSAGE_BUFFER);

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

std::string FileHandler::getFileResource(HttpRequest *request, std::ios_base::openmode mode)
{
	std::string fullPath = getFilePath(request->getResourcePath());

	struct stat file_status;
	if ((stat(fullPath.c_str(), &file_status) == 0) && S_ISDIR(file_status.st_mode))
	{
		if (request->getIsDirListing())
			return buildDirListing(fullPath);

		throw NotFoundException("Directory found but not accessable for directory listing");
	}

	std::ifstream file;
	file.open(fullPath, mode);
	if (!file.is_open() || file.fail() || file.bad())
	{
		std::string message = "Could not open file ";
		message.append(fullPath);
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
	std::string relativePath(DEFAULT_ERRORPAGES_PATH);
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
