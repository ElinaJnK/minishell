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

int	is_builtin(t_cmd *cmd, t_env *lst_env)
{
	if (ft_strcmp(cmd->content, "cd"))
		return (exec_cd(cmd, lst_env));
	if (ft_strcmp(cmd->content, "echo"))
		return (exec_echo(cmd, lst_env));
	if (ft_strcmp(cmd->content, "env"))
		return (exec_env(cmd, lst_env));
	else if (ft_strcmp(cmd->content, "export"))
		return (exec_export(cmd, lst_env));
	else if (ft_strcmp(cmd->content, "pwd"))
		return (exec_pwd(cmd, lst_env));
	else if (ft_strcmp(cmd->content, "unset"))
		return (exec_unset(cmd, lst_env));
	return (EXIT_FAILURE);
}

void	execution(int pid, t_cmd *cmd, t_env *lst_env)
{
	char	*path;
	char	**env;

	env = env_to_tab(lst_env);
	if (is_builtin(cmd->content, lst_env) == EXIT_SUCCESS)
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
// /blabla/ls
// cherche ls
// cmp path pr ls et path donne
// 
// si le path est pas le mm alors il donne a execve

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
	{
		waitpid(pid, &status, 0);
		*exit_status() = WEXITSTATUS(status);
	}
}
