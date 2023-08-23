/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_com.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksadykov <ksadykov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/20 03:55:37 by ksadykov          #+#    #+#             */
/*   Updated: 2023/08/23 22:02:04 by ksadykov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_status(char **env, int pid, int status)
{
	if (status == -1)
	{
		if (errno == EACCES)
			return (free_tab(env), 127);
	}
	if (status < 0 || pid < 0)
		return (free_tab(env), status);
	return (EXIT_FAILURE);
}

int	exec(int pid, t_cmd *cmd, t_all *all)
{
	char	*path;
	char	**env;
	int		status;

	env = env_to_tab(all->env);
	if (!env)
		return (EXIT_FAILURE);
	if (is_paf(cmd->content))
		path = ft_strdup(cmd->content);
	else
		path = get_command_path(cmd->content, all->env);
	if (!path)
	{
		status = execve(cmd->content, cmd->args, env);
		return (check_status(env, pid, status));
	}
	status = execve(path, cmd->args, env);
	if (status == -1)
	{
		if (errno == EACCES)
			return (free_tab(env), 127);
	}
	if (status < 0 || pid < 0)
		return (free(path), free_tab(env), status);
	return (free(path), free_tab(env), status);
}

void	exec_child(t_ast *node, int input_fd, int output_fd, t_all **all)
{
	int	res;

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
	res = exec(0, node->cmd, *all);
	if (res != EXIT_SUCCESS)
	{
		command_not_found(node->cmd->content, *all);
		exit(res);
	}
}

void	choose_exec(t_ast *node, int input_fd, int output_fd, t_all **all)
{
	int	builtin;

	builtin = 0;
	if (is_builtin(node->cmd))
	{
		ft_putstr_fd("22222I was here\n", 2);
		builtin = do_builtin(node->cmd, input_fd, output_fd, *all);
		*exit_status() = builtin;
		free_all(*all);
		exit(*exit_status());
	}
	else
		exec_child(node, input_fd, output_fd, all);
}

void	exec_com(t_ast *node, int input_fd, int output_fd, t_all **all)
{
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
		if (node->cmd->redir_err != 0)
			node->cmd->status = node->cmd->redir_err;
		else
		{
			node->cmd->status = status;
			if (WIFEXITED(status))
				node->cmd->status = WEXITSTATUS(status);
			else if (WIFSIGNALED(status))
				node->cmd->status = 128 + WTERMSIG(status);
		}
		*exit_status() = node->cmd->status;
	}
}
