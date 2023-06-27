#include "../../minishell.h"

int	is_op(char *line, int i)
{
	if (ft_strncmp(line + i, "&&", 2) == 0)
		return (1);
	else if (ft_strncmp(line + i, "||", 2) == 0)
		return (2);
	else if (ft_strncmp(line + i, "|", 1) == 0)
		return (3);
	else if (ft_strncmp(line + i, ">", 1) == 0)
		return (4);
	else if (ft_strncmp(line + i, "<", 1) == 0)
		return (5);
	else if (ft_strncmp(line + i, ">>", 2) == 0)
		return (6);
	else if (ft_strncmp(line + i, "<<", 2) == 0)
		return (7);
	else
		return (0);
}

int	is_sep(char *line, int i)
{
	if (ft_strncmp(line + i, " ", 1) != 0 && ft_strncmp(line + i, "\n", 1) != 0
		&& ft_strncmp(line + i, "\t", 1) != 0)
		return (0);
	return (1);
}

void	add_back_tok(t_token **lst_tok, t_token *new)
{
	t_token	*tmp;

	if (!lst_tok)
		add_back_tok("lst_tok is NULL");
	if (!*lst_tok)
	{
		*lst_tok = new;
		return ;
	}
	tmp = *lst_tok;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new;
}

t_token	*new_token(char *content, int type)
{
	t_token	*tok;

	tok = malloc(sizeof(t_token));
	if (!tok)
		failure("malloc error");
	tok->content = content;
	tok->type = type;
	tok->next = NULL;
	return (tok);
}