/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_type.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksadykov <ksadykov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/20 20:05:54 by ksadykov          #+#    #+#             */
/*   Updated: 2023/08/21 06:17:56 by ksadykov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ast_redir(t_cmd *tokens, t_ast **root, t_ast **current, int start)
{
	t_ast	*node;
	t_ast	*tmp;

	tmp = NULL;
	node = create_node(&tokens[start]);
	if (*current == NULL)
	{
		node->left = *root;
		*current = node;
		*root = node;
	}
	else
	{
		tmp = (*current)->right;
		while (tmp && tmp->right
			&& !(tmp->cmd->type >= 1 && tmp->cmd->type <= 3))
			tmp = tmp->right;
		(*current)->right = node;
		node->left = tmp;
		*current = node;
	}
	if (*root || *current)
		return ;
}

void	ast_op(t_cmd *tokens, t_ast **root, t_ast **current, int start)
{
	t_ast	*node;

	node = create_node(&tokens[start]);
	node->left = *root;
	*current = node;
	*root = NULL;
}

void	ast_repipe(t_ast **root, t_ast **current, t_ast **node)
{
	t_ast	*tmp;
	t_ast	*prev;

	tmp = NULL;
	prev = *root;
	tmp = (*root)->right;
	while (tmp && tmp->right && !(tmp->cmd->type >= 1 && tmp->cmd->type <= 2))
	{
		prev = tmp;
		tmp = tmp->right;
	}
	if (tmp && tmp->cmd->type >= REDIR
		&& tmp->cmd->type <= DREDIR2_E)
	{
		prev->right = *node;
		(*node)->left = tmp;
		*current = *node;
	}
	else
	{
		(*current)->right = *node;
		(*node)->left = tmp;
		*current = *node;
	}
}

void	ast_pipe(t_cmd *tokens, t_ast **root, t_ast **current, int start)
{
	t_ast	*node;

	if (((*root)->cmd->type >= REDIR && (*root)->cmd->type <= DREDIR2_E))
	{
		node = create_node(&tokens[start]);
		node->left = *root;
		*root = node;
		*current = node;
	}
	else
	{
		node = create_node(&tokens[start]);
		if (*current == NULL)
		{
			node->left = *root;
			*current = node;
			*root = node;
		}
		else
			ast_repipe(root, current, &node);
		if (*root || *current)
			return ;
	}
}

void	ast_cmd(t_cmd *tokens, t_ast **root, t_ast **current, int start)
{
	t_ast	*node;

	node = create_node(&tokens[start]);
	if (*current == NULL)
		*root = node;
	else
		(*current)->right = node;
	if (*root || *current)
		return ;
}
