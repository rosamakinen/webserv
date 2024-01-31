
#include "CgiHandler.hpp"

static const char *transferToString(std::map<std::string, std::string> input)
{
	std::string base = "";

	for (std::map<std::string, std::string>::iterator it = input.begin(); it != input.end(); it++)
	{
		base.append(it->first);
		base.append("=");
		base.append(it->second);
		base.append("&");
	   std::cout << base << std::endl;
	}
	base.pop_back();
	const char *string = base.c_str();
	return string;
}


static char **transferToStringArray(std::map<std::string, std::string> input)
{
	char **stringArray = NULL;
	stringArray = new char*[input.size() + 1];
	int i = 0;

	for (std::map<std::string, std::string>::iterator it = input.begin(); it != input.end(); it++)
	{
		std::string base = it->first;
		base.append("=");
		base.append(it->second);
		stringArray[i] = strdup(base.c_str());
		i++;
	}
	stringArray[i] = nullptr;

	return stringArray;
}


static std::map<std::string, std::string> initCgiEnvironment(HttpRequest request)
{
	std::map<std::string, std::string> cgiEnvironment;

	cgiEnvironment["REQUEST_URI"] = request.getUri();
	cgiEnvironment["GATEWAY_INTERFACE"] = GATEWAY_VERSION;
	cgiEnvironment["SERVER_PROTOCOL"] = HTTP_VERSION;
	cgiEnvironment["REQUEST_METHOD"] = request.translateMethod(request.getMethod());
	cgiEnvironment["SCRIPT_FILENAME"] = FileHandler::getFilePath(request.getUri());
	cgiEnvironment["SERVER_SOFTWARE"] = "SillyLittleSoftware/1.0";
	cgiEnvironment["SERVER_NAME"] = "127.0.0.1";
	cgiEnvironment["QUERY_STRING"] = transferToString(request.getParameters());

	return cgiEnvironment;
}

std::string findInterpreterPath(std::string fullPath)
{
	std::ifstream file(fullPath);
	std::string	line;

	if (file.is_open() == true)
	{
		std::getline(file, line);
		if (line.empty() == false)
		{
			if (line.compare(0, 2, SHEBANG) == 0)
				line.erase(0, 2);
		}
		file.close();
	}
	return line;
}

char **getArguments(HttpRequest request)
{
	char **argumentString = new char*[3];

	std::string fullPath = FileHandler::getFilePath(request.getUri());
	std::string shebang = findInterpreterPath(fullPath);

	argumentString[0] = strdup(shebang.c_str());
	argumentString[1] = strdup(fullPath.c_str());
	argumentString[2] = nullptr;

	return argumentString;
}

static int	executeChild(char **argumentString, char **environmentString)
{
	int result = execve(argumentString[0], argumentString, environmentString);
	if (result == -1)
		throw InternalException("Excecve failed");
	return result;
}

int CgiHandler::executeCgi(HttpRequest request)
{

	std::map<std::string, std::string> cgiEnvironment = initCgiEnvironment(request);
	char **environmentString = transferToStringArray(cgiEnvironment);
	char **argumentString = getArguments(request);
	int status = 0;

	int pipe_in[2];
	if (pipe(pipe_in) < 0)
		throw InternalException("Piping input failed");

	int pipe_out[2];
	if (pipe(pipe_out) < 0)
	{
		close(pipe_in[0]);
		close(pipe_in[1]);

		throw InternalException("Piping output failed");
	}

	int pid = fork();
	if (pid == 0)
	{
		dup2(pipe_in[0],  STDIN_FILENO);
		dup2(pipe_out[1],  STDOUT_FILENO);

		close(pipe_in[0]);
		close(pipe_in[1]);
		close(pipe_out[0]);
		close(pipe_out[1]);

		status = executeChild(argumentString, environmentString);

		exit(status);
	}

	//TODO: make a function to free the strArray?

	for (int i = 0; environmentString[i]; i++)
		delete [] environmentString[i];
	delete [] environmentString;

	for (int i = 0; argumentString[i]; i++)
		delete [] argumentString[i];
	delete [] argumentString;

	//TODO: wait for the child process to stop, close pipes etc.
	// Time out, dont wait forever for child to execute
	// Read httprequest from the stdout to write to the client

	return status;
}
