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

int	is_builtin(t_cmd *cmd, int output_fd, t_env *lst_env)
{
	if (ft_strncmp(cmd->content, "cd", ft_strlen(cmd->content) + 1))
		return (exec_cd(cmd));
	if (ft_strncmp(cmd->content, "echo", ft_strlen(cmd->content) + 1))
		return (exec_echo(cmd, output_fd));
	if (ft_strncmp(cmd->content, "env", ft_strlen(cmd->content) + 1))
		return (exec_env(cmd, output_fd, lst_env));
	else if (ft_strncmp(cmd->content, "export", ft_strlen(cmd->content) + 1))
		return (exec_export(cmd, lst_env));
	else if (ft_strncmp(cmd->content, "pwd", ft_strlen(cmd->content) + 1))
		return (exec_pwd(cmd));
	else if (ft_strncmp(cmd->content, "unset", ft_strlen(cmd->content) + 1))
		return (exec_unset(cmd, lst_env));
	return (EXIT_FAILURE);
}

void	execution(int pid, t_cmd *cmd, int output_fd, t_env *lst_env)
{
	char	*path;
	char	**env;

	env = env_to_tab(lst_env);
	if (is_builtin(cmd, output_fd, lst_env) == EXIT_SUCCESS)
		return ;
	if (is_paf(cmd->content))
		path = cmd->content;
	else
		path = get_command_path(cmd->content, lst_env);
	if (!path)
		failure_exec("bash");
	if (execve(path, cmd->args, env) < 0)
		failure("bash");
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
		execution(pid, node->cmd, output_fd, lst_env);
		failure_exec("fork error");
	}
	else
	{
		waitpid(pid, &status, 0);
		*exit_status() = WEXITSTATUS(status);
	}
}
