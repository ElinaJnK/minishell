#include "minishell.h"

void	failure_exec(const char *message)
{
	// if (root)
	// 	free_ast(root);
	// if (cmds)
	// 	free_cmds(cmds, count);
	perror(message);
	exit(EXIT_FAILURE);
}

void	execution(int pid, t_cmd *cmd, t_env *lst_env)
{
	char	*path;
	char	**env;

	env = env_to_tab(lst_env);
	path = get_command_path(cmd->content, lst_env);
	if (!path)
		failure_exec("path");
	if (execve(path, cmd->args, env) < 0)
		failure("execve");
	else if (pid < 0)
		failure("pid");
}

void	exec_com(t_ast *node, int input_fd, int output_fd, t_env *lst_env)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid < 0)
		failure_exec("fork error");
	else if (pid == 0)
	{
		if (input_fd != STDIN_FILENO)
		{
			dup2(input_fd, STDIN_FILENO);
			close(input_fd);
		}
		if (output_fd != STDOUT_FILENO)
		{
			dup2(output_fd, STDOUT_FILENO);
			close(output_fd);
		}
		execution(pid, node->cmd, lst_env);
		failure_exec("fork error");
	}
	else
		waitpid(pid, &status, 0);
}