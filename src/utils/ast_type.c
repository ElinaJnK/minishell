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

void	ast_pipe(t_cmd *tokens, t_ast **root, t_ast **current, int start)
{
	//t_ast	*tmp;
	// t_ast	*node;

	// tmp = NULL;
	// node = NULL;
	// if ((*root)->cmd->type == AND || (*root)->cmd->type == OR)
	// {
	// 	tmp = (*root)->right;
	// 	node = create_node(&tokens[start]);
	// 	(*root)->right = node;
	// 	node->left = tmp;
	// 	*current = node;
	// }
	// else
		// ast_op(tokens, root, current, start);
	ast_redir(tokens, root, current, start);
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
