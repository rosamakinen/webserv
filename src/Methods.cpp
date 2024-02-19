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
		throw BadRequestException("Cannot read the upload file");
	std::cout << "path to the infile: "<< inFilePath << std::endl;


	//get the location, from uri // check the location value, if we have uploadDir, if we have uploadDir store shit there
	std::string fullPath = FileHandler::getFilePath(request.getResourcePath());
	std::cout << "path to the directory: "<< fullPath << std::endl;

	const std::vector<std::string>* uploadLocation = server.getLocationValue("/pictures/", UPLOAD_DIR); // pictures needs to be gotten from uri
	if (uploadLocation == NULL)
		throw ForbiddenException("Upload directory not configured");
	std::string location = uploadLocation->front();
	std::cout << "this is from the vector: " << location << std::endl;

	// throw ForbiddenException("Upload directory not accessible");
	std::string outFilename = getUploadFilename(inFilePath);
	if (outFilename.empty())
		throw BadRequestException("Bad path");
	std::string outFilePath = request.getDirectory();
	outFilePath.append(UPLOAD_DIR);
	outFilePath.append(outFilename);

	std::string fullOutfilePath = FileHandler::getFilePath(outFilePath);
	std::ofstream outputFile(fullOutfilePath);
	if (outputFile.is_open())
	{
		outputFile << getUploadFileContent(inFilePath, std::ios::binary);
		// outputFile << FileHandler::getFileContent(inFilePath, std::ios::binary);
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

