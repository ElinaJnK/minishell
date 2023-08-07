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
		{
			free_cmds(all->cmd, all->count);
			all->cmd = NULL;
		}
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
