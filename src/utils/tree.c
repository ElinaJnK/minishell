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

// void	free_cmd(t_cmd *cmd)
// {
// 	int	i;

// 	i = 0;
// 	if (cmd)
// 	{
// 		free(cmd->content);
// 		while (i <= cmd->nb_args)
// 			free(cmd->args[i++]);
// 		free(cmd->args);
// 		free(cmd);
// 	}
// }

void	free_ast(t_ast *node)
{
	if (node == NULL)
		return ;
	free_ast(node->left);
	free_ast(node->right);
	//free_cmd(node->cmd);
	free(node);
}

void	free_all(t_all *all)
{
	if (all)
	{
		free_ast(all->ast);
		free_cmds(all->cmd, all->count);
		free_lst_env(&all->env);
		free(all->b);
	}
	free(all);
}
