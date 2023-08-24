/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_ast.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ejankovs <ejankovs@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/24 14:47:10 by ksadykov          #+#    #+#             */
/*   Updated: 2023/08/24 19:25:50 by ejankovs         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	exec_and(t_ast **root, int input_fd, int output_fd, t_all *all)
{
	if ((*root)->cmd->n_pipes == 1)
	{
		if ((*root)->left)
			(*root)->left->cmd->n_pipes = 1;
		if ((*root)->right)
			(*root)->right->cmd->n_pipes = 1;
	}
	exec_ast(&((*root)->left), input_fd, output_fd, all);
	wait_op(&all, *root);
	if (*exit_status() == 0)
	{
		exec_ast(&((*root)->right), input_fd, output_fd, all);
		(*root)->right->cmd->status = *exit_status();
	}
}

void	exec_or(t_ast **root, int input_fd, int output_fd, t_all *all)
{
	int	status;
	int	i;

	status = 0;
	i = 0;
	if ((*root)->cmd->n_pipes == 1)
	{
		if ((*root)->left)
			(*root)->left->cmd->n_pipes = 1;
		if ((*root)->right)
			(*root)->right->cmd->n_pipes = 1;
	}
	exec_ast(&((*root)->left), input_fd, output_fd, all);
	wait_op(&all, *root);
	if (*exit_status() != 0)
	{
		exec_ast(&((*root)->right), input_fd, output_fd, all);
		(*root)->right->cmd->status = *exit_status();
	}
}

void	exec_pipe(t_ast **root, int input_fd, int output_fd, t_all *all)
{
	int		pipe_fds[2];
	int		fds[2];

	init_fds(fds, input_fd, output_fd);
	init_pipe(pipe_fds, root);
	(*root)->cmd->pannacota = pipe_fds[0];
	if ((*root)->cmd->pid == 0)
		pipe_child(root, pipe_fds, fds, all);
	else
	{
		pipe_parent1(pipe_fds, root);
		if ((*root)->right->cmd->input != STDIN_FILENO)
			pipe_parent(root, pipe_fds, fds, all);
		else
		{
			if (input_fd != STDIN_FILENO)
				close(input_fd);
			(*root)->right->cmd->input = pipe_fds[0];
			exec_ast(&((*root)->right), pipe_fds[0], output_fd, all);
			close(pipe_fds[0]);
		}
		pipe_parent2(root);
	}
}

void	exec_redir(t_ast **r, int input_fd, int output_fd, t_all *all)
{
	if ((*r)->cmd->n_pipes == 1)
	{
		if ((*r)->left)
			(*r)->left->cmd->n_pipes = 1;
		if ((*r)->right)
			(*r)->right->cmd->n_pipes = 1;
	}
	if ((*r)->cmd->lst_err)
	{
		print_error((*r)->cmd->lst_err, &((*r)->cmd->status));
		(*r)->cmd->redir_err = (*r)->cmd->status;
		*exit_status() = (*r)->cmd->redir_err;
		if ((*r)->left)
			(*r)->left->cmd->redir_err = (*r)->cmd->redir_err;
	}
	if ((*r)->cmd->input < 0 || (*r)->cmd->output < 0)
		return (change_fd(&((*r)->cmd->input), &((*r)->cmd->output)));
	if (!(*r)->left)
		return ;
	if ((*r)->cmd->input != 0 && (*r)->cmd->output != 1)
		exec_ast(&((*r)->left), (*r)->cmd->input, (*r)->cmd->output, all);
	else if ((*r)->cmd->input != STDIN_FILENO)
		exec_ast(&((*r)->left), (*r)->cmd->input, output_fd, all);
	else if ((*r)->cmd->output != STDOUT_FILENO)
		exec_ast(&((*r)->left), input_fd, (*r)->cmd->output, all);
}

void	exec_ast(t_ast **root, int input_fd, int output_fd, t_all *all)
{
	int		builtin;

	if (*root == NULL)
		return ;
	if ((*root)->left == NULL && (*root)->right == NULL
		&& !((*root)->cmd->type >= REDIR && (*root)->cmd->type <= DREDIR2_E))
	{
		if (is_builtin((*root)->cmd) && (*root)->cmd->n_pipes == 0)
		{
			builtin = do_builtin((*root)->cmd, input_fd, output_fd, all);
			*exit_status() = builtin;
		}
		else
			exec_com(*root, input_fd, output_fd, &all);
		return ;
	}
	if ((*root)->cmd->type == AND)
		exec_and(root, input_fd, output_fd, all);
	else if ((*root)->cmd->type == OR)
		exec_or(root, input_fd, output_fd, all);
	else if ((*root)->cmd->type == PIPE)
		exec_pipe(root, input_fd, output_fd, all);
	else if ((*root)->cmd->type >= REDIR && (*root)->cmd->type <= DREDIR2_E)
		exec_redir(root, input_fd, output_fd, all);
}
