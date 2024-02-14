#include "Methods.hpp"

bool Methods::executePost(HttpRequest request)
{
	std::string body = request.getBody();
	if (body.empty())
		return false;

	size_t pos = body.find("=");
	if (pos == std::string::npos)
		return false;

	std::string inFilePath = body.substr(pos + 1, body.length());

	std::string outFilename = UPLOAD_FILENAME;
	std::string outFilePath = request.getDirectory();
	//here we can parse the outfile folder after it's configured
	outFilePath.append(UPLOAD_DIR);
	outFilePath.append(outFilename);

	std::string fullPath = FileHandler::getFilePath(outFilePath);

  	std::ofstream outputFile(fullPath);

  	if (outputFile.is_open()) 
	{  
		std::string content = FileHandler::getFileContent(inFilePath);
    	outputFile << content;

   		outputFile.close();
	}
	else 
		return false;
	
	return true;
}
