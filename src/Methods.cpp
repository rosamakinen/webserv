#include "Methods.hpp"

void Methods::executePost(HttpRequest *request, Server *server)
{
	std::string body = request->getBody();
	if (body.empty())
		throw BadRequestException("Empty body on POST request");

	std::string uploadLocation = request->getUri();
	const std::vector<std::string>* uploadAllowed = server->getLocationValue(uploadLocation, UPLOAD_DIR);
	if (!uploadAllowed || uploadLocation.empty())
		throw ForbiddenException("Upload not configured");
	else if (uploadAllowed->front().compare(TRUE) != 0)
		throw ForbiddenException("Upload not set to true");

	std::string outFilename = request->getFileName();

	std::string outFilePath = request->getDirectory();
	outFilePath.append(outFilename);

	std::string fullPath = FileHandler::getFilePath(outFilePath);
	std::ofstream outputFile(fullPath);
	if (outputFile.is_open() && !outputFile.fail() && !outputFile.bad())
	{
		outputFile << request->getBody();
		outputFile.close();
		return ;
	}

	throw BadRequestException("Uploading a file failed");

}

void Methods::executeDelete(HttpRequest request)
{
	std::string filePath = request.getResourcePath();

	std::string fullPath = FileHandler::getFilePath(filePath);
	if (access(fullPath.c_str(), F_OK) != 0)
		throw NotFoundException("Requested file not found");
	int result = remove(fullPath.c_str());
	if (result == 0)
		return ;
	throw InternalException("Something went wrong");
}

