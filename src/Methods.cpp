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

bool Methods::executePost(HttpRequest request)
{
	std::string body = request.getBody();
	if (body.empty())
		return false;
	size_t pos = body.find("=");
	if (pos == std::string::npos)
		return false;

	std::string inFilePath = body.substr(pos + 1, body.length());
	if (access(inFilePath.c_str(), R_OK) != 0)
		return false;

	//here we can parse the outfile folder after it's configured
	std::string outFilename = getUploadFilename(inFilePath);
	if (outFilename.empty())
		return false;
	std::string outFilePath = request.getDirectory();
	outFilePath.append(UPLOAD_DIR);
	outFilePath.append(outFilename);

	std::string fullPath = FileHandler::getFilePath(outFilePath);
  	std::ofstream outputFile(fullPath);
  	if (outputFile.is_open())
	{
		outputFile << FileHandler::getFileContent(inFilePath, std::ios::binary);
   		outputFile.close();
		return true;
	}
	return false;

}

bool Methods::executeDelete(HttpRequest request)
{
	//here we can parse the outfile folder after it's configured
	std::string filePath = request.getResourcePath();

	std::string fullPath = FileHandler::getFilePath(filePath);
	std::cout << fullPath << std::endl;
	int result = remove(fullPath.c_str());
	if (result == 0)
	{
		return true;
	}
	return false;
}

