/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_ast.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ejankovs <ejankovs@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/21 06:05:23 by ksadykov          #+#    #+#             */
/*   Updated: 2023/08/21 19:04:330 by ejankovs         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	exec_and(t_ast **root, int input_fd, int output_fd, t_all *all)
{
	int status;
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
	// (*root)->left->cmd->redir_err = (*root)->cmd->redir_err;
	exec_ast(&((*root)->left), input_fd, output_fd, all);
	while (i < all->count && &(all->cmd[i]) != (*root)->cmd)
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
	//(*root)->left->cmd->status = *exit_status();
	if (*exit_status() == 0)
	{
		// (*root)->right->cmd->redir_err = (*root)->cmd->redir_err;
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
	//(*root)->left->cmd->redir_err = (*root)->cmd->redir_err;
	exec_ast(&((*root)->left), input_fd, output_fd, all);
	while (i < all->count && &(all->cmd[i]) != (*root)->cmd)
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
	//(*root)->left->cmd->status = *exit_status();
	if (*exit_status() != 0)
	{
		//(*root)->right->cmd->redir_err = (*root)->cmd->redir_err;
		exec_ast(&((*root)->right), input_fd, output_fd, all);
		(*root)->right->cmd->status = *exit_status();
	}
}

void	exec_pipe(t_ast **root, int input_fd, int output_fd, t_all *all)
{
	int		pipe_fds[2];

	if (pipe(pipe_fds) < 0)
		failure_exec("pipe error");
	(*root)->cmd->pid = fork();
	if ((*root)->cmd->pid < 0)
		failure_exec("fork error");
	if ((*root)->cmd->pid == 0)
	{
		close(pipe_fds[0]);	
		// if ((*root)->left)
		// 	(*root)->left->cmd->status = *exit_status();
		//(*root)->left->cmd->redir_err = (*root)->cmd->redir_err;
		pipe_child(root, pipe_fds, input_fd, output_fd, all);
	}
	else
	{
		close(pipe_fds[1]);
		if ((*root)->right)
			(*root)->right->cmd->n_pipes = 1;
		if ((*root)->right->cmd->input != STDIN_FILENO)
		{
			close(pipe_fds[0]);
			// (*root)->right->cmd->redir_err = (*root)->cmd->redir_err;
			exec_ast(&((*root)->right), input_fd, output_fd, all);
		}
		else
		{
			(*root)->right->cmd->input = pipe_fds[0];
			//(*root)->right->cmd->redir_err = (*root)->cmd->redir_err;
			exec_ast(&((*root)->right), pipe_fds[0], output_fd, all);
			close(pipe_fds[0]);
		}
		if ((*root)->right)
			(*root)->right->cmd->status = *exit_status();
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
		printf("redir status : %d\n", (*r)->cmd->redir_err);
		*exit_status() = (*r)->cmd->redir_err;
		if ((*r)->left)
			(*r)->left->cmd->redir_err = (*r)->cmd->redir_err;
	}
	if ((*r)->cmd->input < 0 || (*r)->cmd->output < 0)
	{
		if ((*r)->cmd->input < 0)
			(*r)->cmd->input = STDIN_FILENO;
		if ((*r)->cmd->output < 0)
			(*r)->cmd->output = STDOUT_FILENO;
		// return ;
	}
	if (!(*r)->left)
		return ;
	// (*r)->left->redir_err = (*r)->cmd->status;
	//printf("redir status : %d\n", (*r)->cmd->redir_err);
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
		//printf("content : %s redir_err : %d\n", (*root)->cmd->content, (*root)->cmd->redir_err);
		if (is_builtin((*root)->cmd) && (*root)->cmd->n_pipes == 0)
		{
			builtin = do_builtin((*root)->cmd, output_fd, all);
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
