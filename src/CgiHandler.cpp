
#include "CgiHandler.hpp"

static std::map<std::string, std::string> initCgiEnvironment(HttpRequest request)
{
	std::map<std::string, std::string> cgiEnvironment;

	cgiEnvironment["REQUEST_URI"] = request.getUri();
	cgiEnvironment["GATEWAY_INTERFACE"] = GATEWAY_VERSION;
	cgiEnvironment["SERVER_PROTOCOL"] = HTTP_VERSION;
	cgiEnvironment["REQUEST_METHOD"] = request.translateMethod(request.getMethod());
	cgiEnvironment["SCRIPT_FILENAME"] = FileHandler::getFilePath(request.getUri());

	return cgiEnvironment;
}


static int	executeChild()
{
	return 0;
}

static char **transferToString(std::map<std::string, std::string> cgiEnvironment)
{
	char **environmentString = NULL;
	environmentString = new char*[cgiEnvironment.size() + 1]; 
	int i = 0;

	for (std::map<std::string, std::string>::iterator it = cgiEnvironment.begin(); it != cgiEnvironment.end(); ++it)
	{
		std::string base = it->first;
		base.append("=").append(it->second);
		environmentString[i] = strdup(base.c_str());
		i++;
	}
	environmentString[i] = nullptr;

	return environmentString;
}

int CgiHandler::executeCgi(HttpRequest request)
{
	
	std::map<std::string, std::string> cgiEnvironment = initCgiEnvironment(request);
	char **variableArray = transferToString(cgiEnvironment);

	//TODO: prepare shebang to array[0] prepare cgifile path to array[1] for execve

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

		status = executeChild();
		exit(status);
	}

	//TODO: make a function to free the strArray?

	for (int i = 0; variableArray[i]; i++)
		delete [] variableArray[i];
	delete [] variableArray;

	//TODO: wait for the child process to stop, close pipes etc.
	// Time out, dont wait forever for child to execute
	
	return status;
}
