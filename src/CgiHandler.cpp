
#include "../include/CgiHandler.hpp"
#include "CgiHandler.hpp"


static std::map<std::string, std::string> initFromGetenv()
{
	std::map<std::string, std::string> envMap;
	const char* serverEnviroment[24] =
	{
	"DOCUMENT_ROOT", "GATEWAY_INTERFACE",   
	"HTTP_ACCEPT", "HTTP_ACCEPT_ENCODING",             
	"HTTP_ACCEPT_LANGUAGE", "HTTP_CONNECTION",         
	"HTTP_HOST", "HTTP_USER_AGENT", "PATH",            
	"QUERY_STRING", "REMOTE_ADDR", "REMOTE_PORT",      
	"REQUEST_METHOD", "REQUEST_URI", "SCRIPT_FILENAME",
	"SCRIPT_NAME", "SERVER_ADDR",    
	"SERVER_NAME","SERVER_PORT","SERVER_PROTOCOL",     
	"SERVER_SIGNATURE","SERVER_SOFTWARE" 
	};

	for (int i = 0; i < 24; ++i)
    {
		std::string key = serverEnviroment[i];
		std::string value = getenv(key.c_str());

		envMap.insert(std::pair<std::string, std::string>(key, value));
    }

	return envMap;
}

static std::map<std::string, std::string> initCgiEnvironment(HttpRequest request)
{
	std::map<std::string, std::string> cgiEnvironment = initFromGetenv();
	cgiEnvironment["PATH_INFO"] = request.getUri();
	for (const auto &pair : cgiEnvironment)
		std::cout << "Key: " << pair.first << ", Value: " << pair.second << std::endl;

	return cgiEnvironment;
}


static int	executeChild()
{
	// int result = execve(path_to_command(bin/python-command), command_with_flags(from-te-cgi-bin-directory), environment_variables);
	return 0;
}

int executeCgi(HttpRequest request)
{
	// TODO prepare params
	std::map<std::string, std::string> cgiEnvironment = initCgiEnvironment(request);
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

	//TODO: wait for the child process to stop, close pipes etc.
	// Time out, dont wait forever for child to execute
	return status;
}
