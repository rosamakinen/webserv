
#include "../include/CgiHandler.hpp"

static int	executeChild()
{
	// int result = execve(path_to_command, command_with_flags, environment_variables);
	return 0;
}

int executeCgi()
{
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

	// TODO: wait for the child process to stop, close pipes etc.
	return status;
}
