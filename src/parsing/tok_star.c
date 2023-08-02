#include "minishell.h"

void	check_wildcard(t_token **tmp, t_token **res, int *flag, char *pwd)
{
	if ((*tmp)->type == CMD && (*tmp)->next)
	{
		add_back_tok(res, new_token(ft_strdup((*tmp)->content), (*tmp)->type));
		*tmp = (*tmp)->next;
		while (*tmp && (*tmp)->type == CMD)
		{
			process_wild((*tmp)->content, pwd, res, flag);
			if (*flag == 0)
				add_back_tok(res, new_token(ft_strdup((*tmp)->content),
						(*tmp)->type));
			else if (*flag == 1)
				*flag = 0;
			*tmp = (*tmp)->next;
		}
	}
	else
	{
		add_back_tok(res, new_token(ft_strdup((*tmp)->content), (*tmp)->type));
		*tmp = (*tmp)->next;
	}
}

t_token	*tokenize_crise(t_token *tok)
{
	t_token	*tmp;
	char	*pwd;
	t_token	*res;
	int		flag;

	if (!tok)
		return (NULL);
	pwd = getcwd(NULL, 0);
	if (!pwd)
		return (failure("pwd doesn't work"), NULL);
	tmp = tok;
	flag = 0;
	res = NULL;
	while (tmp)
		check_wildcard(&tmp, &res, &flag, pwd);
	free(pwd);
	if (tok)
		free_lst_tok(&tok);
	return (res);
}
