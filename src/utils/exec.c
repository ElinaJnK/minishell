#include "minishell.h"

// void	failure_exec(const char *message)
// {
// 	// if (root)
// 	// 	free_ast(root);
// 	// if (cmds)
// 	// 	free_cmds(cmds, count);
// 	perror(message);
// 	exit(EXIT_FAILURE);
// }

int	is_paf(char *cmd)
{
	int	i;

	i = 0;
	while (cmd[i])
	{
		if (cmd[i] == '/')
			return (1);
		i++;
	}
	return (0);
}

int	do_builtin(t_cmd *cmd, int output_fd, t_all *all)
{
	if (!ft_strncmp(cmd->content, "cd", ft_strlen(cmd->content) + 1))
		return (exec_cd(cmd));
	else if (!ft_strncmp(cmd->content, "echo", ft_strlen(cmd->content) + 1))
		return (exec_echo(cmd, output_fd));
	else if (!ft_strncmp(cmd->content, "env", ft_strlen(cmd->content) + 1))
		return (exec_env(cmd, output_fd, all->env));
	else if (!ft_strncmp(cmd->content, "exit", ft_strlen(cmd->content) + 1))
		return (exec_exit(cmd, all, output_fd));
	else if (!ft_strncmp(cmd->content, "export", ft_strlen(cmd->content) + 1))
		return (exec_export(cmd, &(all->env)));
	else if (!ft_strncmp(cmd->content, "pwd", ft_strlen(cmd->content) + 1))
		return (exec_pwd(cmd));
	else if (!ft_strncmp(cmd->content, "unset", ft_strlen(cmd->content) + 1))
		return (exec_unset(cmd, &(all->env)));
	return (EXIT_FAILURE);
}

int	is_builtin(t_cmd *cmd)
{
	if (!ft_strncmp(cmd->content, "cd", ft_strlen(cmd->content) + 1)
	|| !ft_strncmp(cmd->content, "echo", ft_strlen(cmd->content) + 1)
	|| !ft_strncmp(cmd->content, "env", ft_strlen(cmd->content) + 1)
	|| !ft_strncmp(cmd->content, "exit", ft_strlen(cmd->content) + 1)
	|| !ft_strncmp(cmd->content, "export", ft_strlen(cmd->content) + 1)
	|| !ft_strncmp(cmd->content, "pwd", ft_strlen(cmd->content) + 1)
	|| !ft_strncmp(cmd->content, "unset", ft_strlen(cmd->content) + 1))
		return (1);
	return (0);
}

int	execution(int pid, t_cmd *cmd, int output_fd, t_all *all)
{
	char	*path;
	char	**env;

	(void)output_fd;
	env = env_to_tab(all->env);
	if (!env)
		return (EXIT_FAILURE);
	if (is_paf(cmd->content))
		path = cmd->content;
	else
		path = get_command_path(cmd->content, all->env);
	if (!path)
	{
		*exit_status() = EXIT_FAILURE;
		failure("bash");
	}
	if (execve(path, cmd->args, env) < 0)
		failure("bash");
	else if (pid < 0)
		failure("pid");
	return (EXIT_FAILURE);
}

void	exec_com(t_ast *node, int input_fd, int output_fd, t_all **all)
{
	pid_t	pid;
	int		status;

	if (is_builtin(node->cmd))
	{
		*exit_status() = do_builtin(node->cmd, output_fd, *all);
		return ;
	}
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
		if (execution(pid, node->cmd, output_fd, *all) == EXIT_FAILURE)
			failure_exec("fork error");
	}
	else
	{
		waitpid(pid, &status, 0);
		*exit_status() = WEXITSTATUS(status);
	}
}
