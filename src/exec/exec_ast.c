/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_ast.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksadykov <ksadykov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/23 12:28:19 by ksadykov          #+#    #+#             */
/*   Updated: 2023/08/23 23:11:01 by ksadykov         ###   ########.fr       */
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
	// printf("0: %d and 1:%d\n", pipe_fds[0], pipe_fds[1]);
	if ((*root)->cmd->pid == 0)
	{
		// close(pipe_fds[0]);
		if ((*root)->left)
			(*root)->left->cmd->n_pipes = 1;
		if ((*root)->left->cmd->output != STDOUT_FILENO)
		{
			close(pipe_fds[0]);
			close(pipe_fds[1]);
			// printf("cmd->left : %d\n", (*root)->left->cmd->output);
			exec_ast(&((*root)->left), input_fd, output_fd, all);
			// if (fds[1]!= STDOUT_FILENO)
			// 	close(fds[1]);
			// if (fds[0]!= STDIN_FILENO)
			// 	close(fds[0]);
		}
		else
		{
			// printf("pipe[1] : %d %s\n", pipe_fds[1], (*root)->left->cmd->content);
			// if (dup2(pipe_fds[1], STDOUT_FILENO) < 0)
			// 	failure_exec("dup2 here");
			// close(pipe_fds[1]);
			(*root)->left->cmd->output = pipe_fds[1];
			exec_ast(&((*root)->left), input_fd, pipe_fds[1], all);
			// printf("done\n");
			if ((*root)->left->cmd->output != STDOUT_FILENO)
				close((*root)->left->cmd->output);
			// if (fds[1]!= STDOUT_FILENO)
			// 	close(fds[1]);
			// if (fds[0]!= STDIN_FILENO)
			// 	close(fds[0]);
			// printf("closed ?? %d\n", close(pipe_fds[1]));
			close(pipe_fds[0]);
			// ft_putstr_fd("2If I am here, you buy me 5 panacotas by tomorrow for lunch\n", 2);
			close(pipe_fds[1]);
		}
		// printf("OYO\n");
		
	// 	int i = 0;
	// while (i < all->count)
	// {
	// 	// printf("CHILD : cmd : %s in : %d out : %d\n", all->cmd[i].content, all->cmd[i].input,all->cmd[i].output);
	// 	if (all->cmd[i].input != STDIN_FILENO && all->cmd[i].input > 0)
	// 	{
	// 		// printf("CHILD should be here\n");
	// 		close(all->cmd[i].input);
	// 	}
	// 	if (all->cmd[i].output != STDOUT_FILENO && all->cmd[i].output > 0)
	// 		close(all->cmd[i].output);
	// 	i++;
	// }
		free_all(all);
		exit(EXIT_SUCCESS);
	}
		// pipe_child(root, pipe_fds, fds, all);
	else
	{
		// pipe_parent1(pipe_fds, root);
		close(pipe_fds[1]);
		if ((*root)->right)
			(*root)->right->cmd->n_pipes = 1;
		if ((*root)->right->cmd->input != STDIN_FILENO)
		{
			close(pipe_fds[0]);
				// if (dup2(input_fd, STDIN_FILENO) < 0)
				// 	failure_exec("dup2 clear");
			if (input_fd != STDIN_FILENO)
				close(input_fd);
			// printf("in parent\n");

			// printf("pipe[0] : %d %s\n", pipe_fds[0], (*root)->right->cmd->content);
			exec_ast(&((*root)->right), input_fd, output_fd, all);
			if ((*root)->right->cmd->input != STDIN_FILENO)
				close((*root)->right->cmd->input);
			if ((*root)->right->cmd->output != STDOUT_FILENO)
				close((*root)->right->cmd->output);
			if (input_fd != STDIN_FILENO)
				close(input_fd);
		}
		else
		{
			// if (output_fd != STDOUT_FILENO)
			// {
			// 	if (dup2(output_fd, STDOUT_FILENO) < 0)
			// 		failure_exec("dup2 clear");
			// 	close(output_fd);
			// }
			// 	if (dup2(pipe_fds[0], STDIN_FILENO) < 0)
			// 		failure_exec("dup2 here");
			// 	close(pipe_fds[0]);
			// printf("pipe[0] : %d\n", pipe_fds[0]);
			if (input_fd != STDIN_FILENO)
				close(input_fd);
			(*root)->right->cmd->input = pipe_fds[0];
			exec_ast(&((*root)->right), pipe_fds[0], output_fd, all);
			close(pipe_fds[0]);
		}
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
		return (change_fd(&((*r)->cmd->input), &((*r)->cmd->output), 
					input_fd, output_fd));
	// change_fd(&((*r)->cmd->input), &((*r)->cmd->output), input_fd, output_fd);
	// if ((*r)->cmd->input < 0 || (*r)->cmd->output < 0)
	// 	change_fd(&((*r)->cmd->input), &((*r)->cmd->output));
	if (!(*r)->left)
		return ;
	if ((*r)->cmd->input != 0 && (*r)->cmd->output != 1)
		exec_ast(&((*r)->left), (*r)->cmd->input, (*r)->cmd->output, all);
	else if ((*r)->cmd->input != STDIN_FILENO)
		exec_ast(&((*r)->left), (*r)->cmd->input, output_fd, all);
	else if ((*r)->cmd->output != STDOUT_FILENO)
		exec_ast(&((*r)->left), input_fd, (*r)->cmd->output, all);
	else
		ft_putstr_fd("~~~~~~My masters didnt consideer me~~~~~~\n", 2);
}

void	exec_ast(t_ast **root, int input_fd, int output_fd, t_all *all)
{
	int		builtin;

	// printf("\n\n cmd : %s input : %d output : %d\n\n", (*root)->cmd->content, input_fd, output_fd);
	if (*root == NULL)
		return ;
	if ((*root)->left == NULL && (*root)->right == NULL
		&& !((*root)->cmd->type >= REDIR && (*root)->cmd->type <= DREDIR2_E))
	{
		if (is_builtin((*root)->cmd)/* && (*root)->cmd->n_pipes == 0*/)
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
	else
		ft_putstr_fd("~~~~~~My masters didnt consideer me~~~~~~\n", 2);
}
