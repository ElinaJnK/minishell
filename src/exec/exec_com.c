#include "minishell.h"

int	do_builtin(t_cmd *cmd, int output_fd, t_all *all)
{
	if (!ft_strncmp(cmd->content, "cd", ft_strlen("cd") + 1))
		return (exec_cd(cmd, output_fd, &(all->env)));
	else if (!ft_strncmp(cmd->content, "echo", ft_strlen("echo") + 1))
		return (exec_echo(cmd, output_fd));
	else if (!ft_strncmp(cmd->content, "env", ft_strlen("env") + 1))
		return (exec_env(cmd, output_fd, all->env));
	else if (!ft_strncmp(cmd->content, "exit", ft_strlen("exit") + 1))
		return (exec_exit(cmd, all, output_fd));
	else if (!ft_strncmp(cmd->content, "export", ft_strlen("export") + 1))
		return (exec_export(cmd, &(all->env), output_fd));
	else if (!ft_strncmp(cmd->content, "pwd", ft_strlen("pwd") + 1))
		return (exec_pwd(cmd, output_fd));
	else if (!ft_strncmp(cmd->content, "unset", ft_strlen("unset") + 1))
		return (exec_unset(cmd, &(all->env)));
	return (EXIT_FAILURE);
}

void	command_not_found(char *msg, t_all *all)
{
	if (errno == ENOENT)
	{
		ft_putstr_fd("bash: ", 2);
		ft_putstr_fd(msg, 2);
		ft_putstr_fd(": command not found\n", 2);
		free_all(all);
		exit(EXIT_FAILURE);
	}
	else
	{
		free_all(all);
		failure_exec("bash");
	}
}

int	exec(int pid, t_cmd *cmd, t_all *all)
{
	char	*path;
	char	**env;

	env = env_to_tab(all->env);
	if (!env)
		return (EXIT_FAILURE);
	if (is_paf(cmd->content))
		path = ft_strdup(cmd->content);
	else
		path = get_command_path(cmd->content, all->env);
	if (!path)
	{
		if (execve(cmd->content, cmd->args, env) < 0 || pid < 0)
			return (free_tab(env), EXIT_FAILURE);
	}
	if (execve(path, cmd->args, env) < 0 || pid < 0)
		return (free(path), free_tab(env), EXIT_FAILURE);
	return (free(path), free_tab(env), EXIT_FAILURE);
}

void	exec_child(t_ast *node, int input_fd, int output_fd, t_all **all)
{
	if (input_fd != STDIN_FILENO)
	{
		if (dup2(input_fd, STDIN_FILENO) < 0)
			failure_exec("dup2 clear");
		close(input_fd);
	}
	if (output_fd != STDOUT_FILENO)
	{
		if (dup2(output_fd, STDOUT_FILENO) < 0)
			failure_exec("dup2 here");
		close(output_fd);
	}
	if (exec(0, node->cmd, *all) == EXIT_FAILURE)
	{
		command_not_found(node->cmd->content, *all);
	}
}

void	choose_exec(t_ast *node, int input_fd, int output_fd, t_all **all)
{
	int	builtin;

	builtin = 0;
	if (is_builtin(node->cmd))
	{
		builtin = do_builtin(node->cmd, output_fd, *all);
		*exit_status() = builtin;
		free_all(*all);
		exit(*exit_status());
	}
	else
		exec_child(node, input_fd, output_fd, all);
}

void	exec_com(t_ast *node, int input_fd, int output_fd, t_all **all)
{
	//pid_t	pid;
	int		status;

	node->cmd->pid = fork();
	if (node->cmd->pid < 0)
		failure_exec("fork error");
	else if (node->cmd->pid == 0)
	{
		sig_child();
		choose_exec(node, input_fd, output_fd, all);
	}
	else
	{
		waitpid(node->cmd->pid, &status, 0);
		// if (*exit_status() != 130 && *exit_status() != 131)
		// 	*exit_status() = WEXITSTATUS(status);
	}
}
