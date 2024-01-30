
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


static int	executeChild(char **cgiArguments, char **environmentString)
{
	std::cout << cgiArguments << environmentString << std::endl;
	// int result = execve(path_to_command(bin/python-command), command_with_flags(from-te-cgi-bin-directory), environment_variables);
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

std::string findShebang(std::string fullPath)
{
	std::ifstream file(fullPath);
	std::string	line;
	
	if (file.is_open() == true)
	{
		std::getline(file, line);
		if (line.empty() == false)
		{
			if (line.compare(0, 2, PREFIX_SHEBANG) == 0)
				line.erase(0, 2);
		}
		file.close();
	}
	return line;
}

char **getArguments(HttpRequest request)
{
	//TODO: prepare shebang, to array[0], check that we can execute, prepare cgifile path to array[1] for execve.
	//for execve you give (cgiArguments[0], cgiArguments, environmentString;)

	char **argumentString = new char*[3];

	std::string fullPath = FileHandler::getFilePath(request.getUri());
	std::string shebang = findShebang(fullPath);
	std::cout << "shebang isss: " << shebang << std::endl;

	argumentString[0] = strdup(shebang.c_str());
	argumentString[1] = strdup(fullPath.c_str());
	argumentString[2] = nullptr;

	return argumentString;
}

int CgiHandler::executeCgi(HttpRequest request)
{
	
	std::map<std::string, std::string> cgiEnvironment = initCgiEnvironment(request);
	char **environmentString = transferToString(cgiEnvironment);
	char **argumentString = getArguments(request);
	
	for (int i = 0; argumentString[i]; i++)
		std::cout << "argument String::" << argumentString[i] << std::endl;


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
	
	return status;
}
