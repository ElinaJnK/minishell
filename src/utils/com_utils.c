/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   com_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ejankovs <ejankovs@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/23 12:41:14 by ksadykov          #+#    #+#             */
/*   Updated: 2023/08/24 19:28:28 by ejankovs         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	do_builtin(t_cmd *cmd, int input_fd, int output_fd, t_all *all)
{
	if (input_fd != STDIN_FILENO)
		close(input_fd);
	if (!ft_strncmp(cmd->content, "cd", ft_strlen("cd") + 1))
		return (exec_cd(cmd, output_fd, all));
	else if (!ft_strncmp(cmd->content, "echo", ft_strlen("echo") + 1))
		return (exec_echo(cmd, output_fd));
	else if (!ft_strncmp(cmd->content, "env", ft_strlen("env") + 1))
		return (exec_env(cmd, output_fd, all->env));
	else if (!ft_strncmp(cmd->content, "exit", ft_strlen("exit") + 1))
		return (exec_exit(cmd, all));
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
	}
	else if (errno == EACCES)
	{
		ft_putstr_fd("bash: ", 2);
		ft_putstr_fd(msg, 2);
		ft_putstr_fd(": Permission denied\n", 2);
		free_all(all);
	}
	else
		free_all(all);
}

void	print_error(t_token *lst_err, int *status)
{
	t_token	*tmp;

	tmp = lst_err;
	while (tmp)
	{
		ft_putstr_fd("bash: ", 2);
		ft_putstr_fd(tmp->content, 2);
		*exit_status() = tmp->type;
		*status = tmp->type;
		tmp = tmp->next;
	}
}

void	pipe_child(t_ast **root, int *pipe_fds, int *fds, t_all *all)
{
	if ((*root)->left)
			(*root)->left->cmd->n_pipes = 1;
	if ((*root)->left->cmd->output != STDOUT_FILENO)
	{
		close(pipe_fds[1]);
		close(pipe_fds[0]);
		exec_ast(&((*root)->left), fds[0], fds[1], all);
	}
	else
	{
		(*root)->left->cmd->output = pipe_fds[1];
		if (ft_strncmp((*root)->left->cmd->content, "echo",
				ft_strlen("echo") + 1))
			close(pipe_fds[0]);
		exec_ast(&((*root)->left), fds[0], pipe_fds[1], all);
		if ((*root)->left->cmd->output != STDOUT_FILENO)
			close((*root)->left->cmd->output);
		if (!ft_strncmp((*root)->left->cmd->content, "echo",
				ft_strlen("echo") + 1))
			close(pipe_fds[0]);
		close(pipe_fds[1]);
	}
	free_all(all);
	exit(EXIT_SUCCESS);
}

void	init_fds(int *fds, int input_fd, int output_fd)
{
	fds[0] = input_fd;
	fds[1] = output_fd;
}
