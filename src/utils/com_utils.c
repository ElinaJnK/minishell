/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   com_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ejankovs <ejankovs@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/20 20:05:30 by ksadykov          #+#    #+#             */
/*   Updated: 2023/08/21 18:418:4047 by ejankovs         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	do_builtin(t_cmd *cmd, int output_fd, t_all *all)
{
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
		exit(127);
	}
	else
	{
		free_all(all);
		failure_exec("bash");
	}
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
		//printf("exit_status : %d\n", *exit_status());
		tmp = tmp->next;
	}
}

void	pipe_child(t_ast **root, int *pipe_fds, int input_fd, int output_fd, t_all *all)
{
	if ((*root)->left)
			(*root)->left->cmd->n_pipes = 1;
	if ((*root)->left->cmd->output != STDOUT_FILENO)
	{
		close(pipe_fds[1]);
		exec_ast(&((*root)->left), input_fd, output_fd, all);
	}
	else
	{
		(*root)->left->cmd->output = pipe_fds[1];
		exec_ast(&((*root)->left), input_fd, pipe_fds[1], all);
	}
	free_all(all);
	exit(EXIT_SUCCESS);
}
