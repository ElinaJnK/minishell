#include "minishell.h"

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

	if (!*lst_tok)
		return ;
	while (*lst_tok)
	{
		tmp = (*lst_tok)->next;
		if ((*lst_tok)->content)
			free((*lst_tok)->content);
		free(*lst_tok);
		*lst_tok = tmp;
	}
}

int	is_heredoc(t_token *lst_tok)
{
	t_token	*tmp;

	tmp = lst_tok;
	if (!lst_tok)
		return (0);
	while (tmp->next)
		tmp = tmp->next;
	if (tmp->type == DREDIR2 || tmp->type == DREDIR2_E)
		return (1);
	return (0);
}

t_token	*last_elem(t_token *lst_tok)
{
	t_token	*tmp;

	tmp = lst_tok;
	if (!lst_tok)
		return (NULL);
	while (tmp->next)
		tmp = tmp->next;
	return (tmp);
}
