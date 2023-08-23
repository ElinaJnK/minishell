/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_ast_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksadykov <ksadykov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/23 12:54:07 by ksadykov          #+#    #+#             */
/*   Updated: 2023/08/23 13:04:52 by ksadykov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	change_fd(int *in, int *out)
{
	if (*in < 0)
		*in = STDIN_FILENO;
	if (*out < 0)
		*out = STDOUT_FILENO;
}

void	init_pipe(int *pipe_fds, t_ast **root)
{
	if (pipe(pipe_fds) < 0)
		failure_exec("pipe error");
	(*root)->cmd->pid = fork();
	if ((*root)->cmd->pid < 0)
		failure_exec("fork error");
}

void	pipe_parent1(int *pipe_fds, t_ast **root)
{
	close(pipe_fds[1]);
	if ((*root)->right)
		(*root)->right->cmd->n_pipes = 1;
}

void	pipe_parent2(t_ast **root)
{
	if ((*root)->right)
	{
		(*root)->right->cmd->status = *exit_status();
		if ((*root)->right->cmd->redir_err != 0)
		{
			(*root)->cmd->redir_err = (*root)->right->cmd->redir_err;
			*exit_status() = (*root)->right->cmd->redir_err;
		}
	}
}

void	wait_op(t_all **a, t_ast *root)
{
	t_all	*all;
	int		i;
	int		status;

	all = *a;
	status = 0;
	i = 0;
	while (i < all->count && &(all->cmd[i]) != root->cmd)
	{
		if (all->cmd[i].pid > 0)
		{
			if (waitpid(all->cmd[i].pid, &status, 0) == -1)
				status = all->cmd[i].status;
			if (WIFEXITED(status))
				*exit_status() = WEXITSTATUS(status);
			else if (WIFSIGNALED(status))
			{
				*exit_status() = 128 + WTERMSIG(status);
			}
			all->cmd[i].pid = -42;
		}
		if (all->cmd[i].redir_err != 0)
			*exit_status() = all->cmd[i].redir_err;
		i++;
	}
}
