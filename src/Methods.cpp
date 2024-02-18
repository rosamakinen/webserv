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

void Methods::executePost(HttpRequest request)
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

	// here we should parse the uri to be the upload directory,
	// but that needs big changes, so it will be done later
	std::string outFilename = getUploadFilename(inFilePath);
	if (outFilename.empty())
		throw BadRequestException("Bad path");
	std::string outFilePath = request.getDirectory();
	outFilePath.append(UPLOAD_DIR);
	outFilePath.append(outFilename);

	std::string fullPath = FileHandler::getFilePath(outFilePath);
	std::ofstream outputFile(fullPath);
	if (outputFile.is_open())
	{
		outputFile << FileHandler::getFileContent(inFilePath, std::ios::binary);
		outputFile.close();
		return ;
	}
	throw InternalException("Something went wrong");

}

void Methods::executeDelete(HttpRequest request)
{
	//here we can parse the outfile folder after it's working with uri
	std::string filePath = request.getResourcePath();

	std::string fullPath = FileHandler::getFilePath(filePath);
	int result = remove(fullPath.c_str());
	if (result == 0)
		return ;
	throw InternalException("Something went wrong");
}

