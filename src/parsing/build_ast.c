#include "minishell.h"

void	ast_in_par(t_cmd *tokens, t_ast **root, t_ast **cur, t_border *b)
{
	int		j;
	int		n;
	int		p_count;
	t_ast	*subtree;

	p_count = 1;
	j = *b->start;
	while (j++ <= b->end)
	{
		if (tokens[j].type == OPEN_PAR)
			p_count++;
		else if (tokens[j].type == CLOSE_PAR)
			p_count--;
		if (p_count == 0)
			break ;
	}
	n = *b->start + 1;
	subtree = build_ast(tokens, &(t_border){&n, j - 1});
	if (*cur == NULL)
		*root = subtree;
	else
		(*cur)->right = subtree;
	*b->start = j;
	if (*root || *cur)
		return ;
}

void	otherwise(t_cmd *tokens, t_ast **root, t_ast **current, int i)
{
	if (tokens[i].type == AND || tokens[i].type == OR)
		ast_op(tokens, root, current, i);
	else if ((tokens[i].type >= REDIR && tokens[i].type <= DREDIR2_E))
		ast_redir(tokens, root, current, i);
	else if (tokens[i].type == PIPE)
		ast_pipe(tokens, root, current, i);
	else
		ast_cmd(tokens, root, current, i);
}

t_ast	*build_ast(t_cmd *tokens, t_border *b)
{
	t_ast	*root;
	t_ast	*current;
	int		i;

	if (*b->start > b->end)
		return (NULL);
	root = NULL;
	current = NULL;
	i = *b->start;
	while (i <= b->end)
	{
		if (!root)
			root = current;
		if (tokens[i].type == OPEN_PAR)
			ast_in_par(tokens, &root, &current, &(t_border){&i, b->end});
		else
			otherwise(tokens, &root, &current, i);
		i++;
	}
	return (root);
}
