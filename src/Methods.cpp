#include "Methods.hpp"

static std::string getUploadFilename(std::string path)
{
	std::size_t found = path.find_last_of(BACKSLASH);
	if (found != std::string::npos)
	{
		std::string filename = path.substr(found + 1, path.length());
		return filename;
	}
	return "";
}

static std::string getUploadFileContent (std::string path, std::ios_base::openmode mode)
{
	std::ifstream file(path);
	if (!file.is_open() || file.fail() || file.bad())
	{
		std::string message = "Could not open file ";
		message.append(path);
		message.append(" for reading");
		throw FileException(message);
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

void Methods::executePost(Server server, HttpRequest request)
{
	std::string body = request.getBody();
	if (body.empty())
		throw BadRequestException("empty body on POST request");
	size_t pos = body.find("=");
	if (pos == std::string::npos)
		throw BadRequestException("Bad query");

	std::string inFilePath = body.substr(pos + 1, body.length());
	if (access(inFilePath.c_str(), R_OK) != 0)
		throw BadRequestException("Could not read the upload file");

	std::string uploadLocation = request.getUri();
	const std::vector<std::string>* uploadAllowed = server.getLocationValue(uploadLocation, UPLOAD);
	if (uploadAllowed == NULL)
		throw ForbiddenException("Upload not configured");

	std::string outFilename = getUploadFilename(inFilePath);
	if (outFilename.empty())
		throw BadRequestException("Bad path for upload file");

	std::string outFilePath = request.getDirectory();
	outFilePath.append(outFilename);
	std::ofstream outputFile(FileHandler::getFilePath(outFilePath));
	if (outputFile.is_open())
	{
		outputFile << getUploadFileContent(inFilePath, std::ios::binary);
		outputFile.close();
		return ;
	}
	throw BadRequestException("Something went wrong");

}

void Methods::executeDelete(HttpRequest request)
{
	std::string path = request.getResourcePath();
	std::string fullPath = FileHandler::getFilePath(path);
	int result = remove(fullPath.c_str());
	if (result == 0)
		return ;
	throw BadRequestException("Could not delete requested file");
}

