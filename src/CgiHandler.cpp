
#include "../include/CgiHandler.hpp"

static std::string transferToString(std::map<std::string, std::string> input)
{
	std::string base = "";

	if (input.empty())
		return base;
	for (std::map<std::string, std::string>::iterator it = input.begin(); it != input.end(); it++)
	{
		base.append(it->first);
		base.append("=");
		base.append(it->second);
		base.append("&");
	}
	base.pop_back();

	return base;
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

static std::string getQueryString(HttpRequest request)
{
	std::string query;

	switch (request.getMethod())
	{
		case Util::METHOD::CGI_GET:
		{
			query = transferToString(request.getParameters());
			return query;
			break;
		}

		case Util::METHOD::CGI_POST:
		{
			query = request.getBody();
			return query;
			break;
		}

		default:
			throw BadRequestException("Bad query");
	}
	return nullptr;
}

static std::map<std::string, std::string> initCgiEnvironment(HttpRequest request)
{
	std::map<std::string, std::string> cgiEnvironment;

	cgiEnvironment["REQUEST_URI"] = request.getUri();
	cgiEnvironment["GATEWAY_INTERFACE"] = GATEWAY_VERSION;
	cgiEnvironment["SERVER_PROTOCOL"] = HTTP_VERSION;
	cgiEnvironment["REQUEST_METHOD"] = Util::translateMethod(request.getMethod());
	cgiEnvironment["SCRIPT_FILENAME"] = FileHandler::getFilePath(request.getResourcePath());
	cgiEnvironment["SERVER_SOFTWARE"] = "SillyLittleSoftware/1.0"; //fetch from config?
	cgiEnvironment["SERVER_NAME"] = "127.0.0.1"; //fetch from config?
	cgiEnvironment["QUERY_STRING"] = getQueryString(request);
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

	std::string fullPath = FileHandler::getFilePath(request.getResourcePath());
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
		exit(1);
	return result;
}


std::string	readCgi(int pipe_out)
{
	std::string	response;
	char		buffer[5000];
	int			readBytes;

	while (1)
	{
		readBytes = read(pipe_out, buffer, sizeof(buffer));
		if (readBytes <= 0)
			break;
		response.append(buffer);
	}
	close(pipe_out);
	return response;
}

std::string	CgiHandler::executeCgi(HttpRequest request)
{
	std::map<std::string, std::string> cgiEnvironment = initCgiEnvironment(request);
	char **environmentString = transferToStringArray(cgiEnvironment);
	char **argumentString = getArguments(request);
	std::string response;

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
		close(pipe_in[1]);
		close(pipe_out[0]);

		dup2(pipe_in[0],  STDIN_FILENO);
		dup2(pipe_out[1],  STDOUT_FILENO);

		close(pipe_in[0]);
		close(pipe_out[1]);

		executeChild(argumentString, environmentString);
		exit(0);
	}
	else
	{
		int status;
		waitpid(pid, &status, 0);
		close(pipe_in[0]);
		close(pipe_in[1]);
		close(pipe_out[1]);
		response = readCgi(pipe_out[0]);
	}

	//TODO: make a function to free the string arrays?

	for (int i = 0; environmentString[i]; i++)
		delete [] environmentString[i];
	delete [] environmentString;

	for (int i = 0; argumentString[i]; i++)
		delete [] argumentString[i];
	delete [] argumentString;

	return response;
}
