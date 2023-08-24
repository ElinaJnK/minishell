/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ejankovs <ejankovs@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/20 20:10:01 by ksadykov          #+#    #+#             */
/*   Updated: 2023/08/24 18:22:40 by ejankovs         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_ast	*create_node(t_cmd *cmd)
{
	t_ast	*node;

	node = (t_ast *) malloc(sizeof(t_ast));
	if (!node)
		return (NULL);
	node->cmd = cmd;
	node->left = NULL;
	node->right = NULL;
	return (node);
}

void	free_ast(t_ast *node)
{
	if (node == NULL)
		return ;
	if (node->left)
		free_ast(node->left);
	if (node->right)
		free_ast(node->right);
	if (node)
		free(node);
}

void	free_and_close(t_all *all)
{
	int	i;

	i = 0;
	while (i < all->count)
	{
		if (all->cmd[i].input != STDIN_FILENO && all->cmd[i].input > 0)
			close(all->cmd[i].input);
		if (all->cmd[i].output != STDOUT_FILENO && all->cmd[i].output > 0)
			close(all->cmd[i].output);
		if (all->cmd[i].pannacota != -42)
		{
			close(all->cmd[i].pannacota);
			all->cmd[i].pannacota = -42;
		}
		i++;
	}
	free_cmds(all->cmd, all->count);
	all->cmd = NULL;
}

void	free_all(t_all *all)
{
	if (all)
	{
		if (all->ast)
		{
			free_ast(all->ast);
			all->ast = NULL;
		}
		if (all->cmd)
			free_and_close(all);
		if (all->env)
		{
			free_lst_env(&all->env);
			all->env = NULL;
		}
		free(all->b);
		free(all->prompt_good);
		free(all->prompt_bad);
	}
	free(all);
}

void	pipe_parent(t_ast **root, int *pipe_fds, int *fds, t_all *all)
{
	close(pipe_fds[0]);
	close(pipe_fds[1]);
	exec_ast(&((*root)->right), fds[0], fds[1], all);
	if (fds[0] != STDIN_FILENO)
		close(fds[0]);
	if (fds[1] != STDOUT_FILENO)
		close(fds[1]);
}
