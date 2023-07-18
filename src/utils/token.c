#include "minishell.h"

int	get_type(char *tok)
{
	int	op;
	int	meta;

	if (!tok)
		return (-1);
	op = is_op(tok);
	meta = is_fb(tok);
	if (op)
		return (op);
	if (meta)
		return (meta);
	return (CMD);
}

int	is_op(char *line)
{
	if (ft_strncmp(line, "&&", 2) == 0)
		return (AND);
	else if (ft_strncmp(line, "||", 2) == 0)
		return (OR);
	return (0);
}

int	is_fb(char *line)
{
	if (ft_strncmp(line, "|", 1) == 0)
		return (PIPE);
	else if (ft_strncmp(line, ">", 1) == 0)
		return (REDIR);
	else if (ft_strncmp(line, "<", 1) == 0)
		return (REDIR2);
	else if (ft_strncmp(line, ">>", 2) == 0)
		return (DREDIR);
	else if (ft_strncmp(line, "<<", 2) == 0)
		return (DREDIR2);
	else if (ft_strncmp(line, "(", 1) == 0)
		return (OPEN_PAR);
	else if (ft_strncmp(line, ")", 1) == 0)
		return (CLOSE_PAR);
	return (CMD);
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
	if (new && new->type == CMD)
		new->type = get_type(new->content);
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
