#include "Methods.hpp"

void Methods::executePost(HttpRequest *request, Server *server)
{
	std::string body = request->getBody();
	if (body.empty())
		throw BadRequestException("empty body on POST request");

	// add checking if the upload folder is configured
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
	if (outputFile.is_open())
	{
		outputFile << request->getBody();
		outputFile.close();
		return ;
	}
	throw BadRequestException("Something went wrong");

}

void Methods::executeDelete(HttpRequest request)
{
	std::string filePath = request.getResourcePath();

	std::string fullPath = FileHandler::getFilePath(filePath);
	int result = remove(fullPath.c_str());
	if (result == 0)
		return ;
	throw BadRequestException("Something went wrong");
}

