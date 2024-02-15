#include "Methods.hpp"

// static std::string getUploadFilename(std::string path)
// {
// 	std::size_t found = path.find_last_of(BACKSLASH);
// 	std::cout << found << std::endl;

// }
bool Methods::executePost(HttpRequest request)
{
	std::string body = request.getBody();
	if (body.empty())
		throw BadRequestException("Empty query body");
	size_t pos = body.find("=");
	if (pos == std::string::npos)
		throw BadRequestException("Bad query");

	std::string inFilePath = body.substr(pos + 1, body.length());

	// std::string outFilename = getUploadFilename(inFilePath);
	std::string outFilename = "moi";
	std::string outFilePath = request.getDirectory();
	//here we can parse the outfile folder after it's configured
	outFilePath.append(UPLOAD_DIR);
	outFilePath.append(outFilename);

	std::string fullPath = FileHandler::getFilePath(outFilePath);

  	std::ofstream outputFile(fullPath);
  	if (outputFile.is_open()) 
	{  
		outputFile << FileHandler::getFileContent(inFilePath, std::ios::binary);
   		outputFile.close();
	}
	else 
		throw FileException("Failed to open resource");
	
	return true;
}
