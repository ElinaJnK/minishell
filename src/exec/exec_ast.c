/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_ast.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksadykov <ksadykov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/20 04:06:17 by ksadykov          #+#    #+#             */
/*   Updated: 2023/08/20 04:06:20 by ksadykov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	exec_and(t_ast *root, int input_fd, int output_fd, t_all *all)
{
	if (root->cmd->n_pipes == 1)
	{
		if (root->left)
			root->left->cmd->n_pipes = 1;
		if (root->right)
			root->right->cmd->n_pipes = 1;
	}
	exec_ast(root->left, input_fd, output_fd, all);
	if (*exit_status() == 0)
		exec_ast(root->right, input_fd, output_fd, all);
}

void	exec_or(t_ast *root, int input_fd, int output_fd, t_all *all)
{
	if (root->cmd->n_pipes == 1)
	{
		if (root->left)
			root->left->cmd->n_pipes = 1;
		if (root->right)
			root->right->cmd->n_pipes = 1;
	}
	exec_ast(root->left, input_fd, output_fd, all);
	if (*exit_status() != 0)
		exec_ast(root->right, input_fd, output_fd, all);
}

void	exec_pipe(t_ast *root, int input_fd, int output_fd, t_all *all)
{
	int		pipe_fds[2];

	if (pipe(pipe_fds) < 0)
		failure_exec("pipe error");
	root->cmd->pid = fork();
	if (root->cmd->pid < 0)
		failure_exec("fork error");
	if (root->cmd->pid == 0)
	{
		close(pipe_fds[0]);
		if (output_fd != STDOUT_FILENO)
			close(output_fd);
		pipe_child(root, pipe_fds, input_fd, all);
	}
	else
	{
		close(pipe_fds[1]);
		if (input_fd != STDIN_FILENO)
			close(input_fd);
		if (root->right)
			root->right->cmd->n_pipes = 1;
		root->right->cmd->input = pipe_fds[0];
		exec_ast(root->right, pipe_fds[0], output_fd, all);
		close(pipe_fds[0]);
	}
}

void	exec_redir(t_ast *root, int input_fd, int output_fd, t_all *all)
{
	if (root->cmd->n_pipes == 1)
	{
		if (root->left)
			root->left->cmd->n_pipes = 1;
		if (root->right)
			root->right->cmd->n_pipes = 1;
	}
	if (root->left)
	{
		if (root->cmd->lst_err)
			print_error(root->cmd->lst_err);
		if (root->cmd->input < 0 || root->cmd->output < 0)
			return ;
		if (root->cmd->input != STDIN_FILENO && root->cmd->output
			!= STDOUT_FILENO)
			exec_ast(root->left, root->cmd->input, root->cmd->output, all);
		else if (root->cmd->input != STDIN_FILENO)
			exec_ast(root->left, root->cmd->input, output_fd, all);
		else if (root->cmd->output != STDOUT_FILENO)
			exec_ast(root->left, input_fd, root->cmd->output, all);
	}
}

void	exec_ast(t_ast *root, int input_fd, int output_fd, t_all *all)
{
	int		builtin;

	if (root == NULL)
		return ;
	if (root->left == NULL && root->right == NULL
		&& !(root->cmd->type >= REDIR && root->cmd->type <= DREDIR2_E))
	{
		if (is_builtin(root->cmd) && root->cmd->n_pipes == 0)
		{
			builtin = do_builtin(root->cmd, output_fd, all);
			*exit_status() = builtin;
		}
		else
			exec_com(root, input_fd, output_fd, &all);
		return ;
	}
	if (root->cmd->type == AND)
		exec_and(root, input_fd, output_fd, all);
	else if (root->cmd->type == OR)
		exec_or(root, input_fd, output_fd, all);
	else if (root->cmd->type == PIPE)
		exec_pipe(root, input_fd, output_fd, all);
	else if (root->cmd->type >= REDIR && root->cmd->type <= DREDIR2_E)
		exec_redir(root, input_fd, output_fd, all);
}
