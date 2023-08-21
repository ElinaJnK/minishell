/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksadykov <ksadykov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/20 20:10:01 by ksadykov          #+#    #+#             */
/*   Updated: 2023/08/21 06:18:19 by ksadykov         ###   ########.fr       */
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
