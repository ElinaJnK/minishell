#include "minishell.h"

int	get_type(char *tok)
{
	int	op;
	int	meta;

	if (!tok)
		return (0);
	op = is_op(tok);
	meta = is_fb(tok);
	if (op)
		return (op);
	if (meta)
		return (meta);
	return (0);
}

int	is_op(char *line)
{
	if (ft_strncmp(line, "&&", 2) == 0)
		return (1);
	else if (ft_strncmp(line, "||", 2) == 0)
		return (2);
	return (0);
}

int	is_fb(char *line)
{
	if (ft_strncmp(line, "|", 1) == 0)
		return (3);
	else if (ft_strncmp(line, ">", 1) == 0)
		return (4);
	else if (ft_strncmp(line, "<", 1) == 0)
		return (5);
	else if (ft_strncmp(line, ">>", 2) == 0)
		return (6);
	else if (ft_strncmp(line, "<<", 2) == 0)
		return (7);
	else if (ft_strncmp(line, "(", 1) == 0)
		return (8);
	else if (ft_strncmp(line, ")", 1) == 0)
		return (9);
	return (0);
}

void	add_back_tok(t_token **lst_tok, t_token *new)
{
	t_token	*tmp;

	if (!lst_tok)
		failure("lst_tok is NULL");
	if (*lst_tok == NULL)
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

void	free_lst_tok(t_token **lst_tok)
{
	t_token	*tmp;

	if (!lst_tok)
		return ;
	while (*lst_tok)
	{
		tmp = (*lst_tok)->next;
		free((*lst_tok)->content);
		free(*lst_tok);
		*lst_tok = tmp;
	}
}
