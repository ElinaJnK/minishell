#include "minishell.h"

void	failure_group(const char *message, t_ast *ast)
{
	free_ast(ast);
	perror(message);
	exit(EXIT_FAILURE);
}

// t_ast	*new_node(t_token *tok)
// {
// 	t_ast	*node;

// 	node = (t_ast *)malloc(sizeof(t_ast));
// 	if (!node)
// 		failure("Error: malloc");
// 	node->tok = tok;
// 	node->type = tok->type;
// 	node->left = NULL;
// 	node->right = NULL;
// 	return (node);
// }

// void	free_ast(t_ast *a)
// {
// 	if (!a)
// 		return ;
// 	free_ast(a->left);
// 	free_ast(a->right);
// 	free(a);
// }
