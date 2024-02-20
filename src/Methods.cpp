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

void Methods::executePost(HttpRequest request, Server server)
{
	std::string body = request.getBody();
	if (body.empty())
		throw BadRequestException("empty body on POST request");
	size_t pos = body.find("=");
	if (pos == std::string::npos)
		throw BadRequestException("Bad query");

	std::string inFilePath = body.substr(pos + 1, body.length());
	if (access(inFilePath.c_str(), R_OK) != 0)
		throw BadRequestException("Cannot read the upload file");

	// add checking if the upload folder is configured
	std::string uploadLocation = request.getUri();
	const std::vector<std::string>* uploadAllowed = server.getLocationValue(uploadLocation, UPLOAD);
	if (!uploadAllowed || uploadLocation.empty())
		throw ForbiddenException("Upload not configured");
	else if (uploadAllowed->front().compare(TRUE) != 0)
		throw ForbiddenException("Upload not set to true");


	std::string outFilename = getUploadFilename(inFilePath);
	if (outFilename.empty())
		throw BadRequestException("Bad path");
	std::string outFilePath = request.getDirectory();
	outFilePath.append(outFilename);

	std::string fullPath = FileHandler::getFilePath(outFilePath);
	std::ofstream outputFile(fullPath);
	std::cout << "outfilepath: " << fullPath << std::endl;
	std::cout << "infilepath: " << inFilePath << std::endl;
	if (outputFile.is_open())
	{
		outputFile << FileHandler::getUploadFileContent(inFilePath, std::ios::binary);
		outputFile.close();
		return ;
	}
	throw InternalException("Something went wrong");

}

void Methods::executeDelete(HttpRequest request)
{
	std::string filePath = request.getResourcePath();

	std::string fullPath = FileHandler::getFilePath(filePath);
	std::cout << "deleting :" << fullPath << std::endl;
	int result = remove(fullPath.c_str());
	if (result == 0)
		return ;
	throw InternalException("Something went wrong");
}

