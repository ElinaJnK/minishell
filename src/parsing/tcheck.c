#include "minishell.h"

void	check_tok(t_token *lst_tok)
{
	t_token	*tmp;
	int		p;

	tmp = lst_tok;
	p = 0;
	if (!tmp || ((tmp->type >= 1 && tmp->type <= 3) || tmp->type == 9))
		failure_parse("tcheque error", lst_tok);
	while (tmp && tmp->next)
	{
		if (tmp->type == 8)
			p++;
		if (tmp->type == 9)
			p--;	
		if (tmp->next->type == 8 && tmp->type == 0)
			failure_parse("tcheque error", lst_tok);
		if (tmp->type == 9 && tmp->next->type == 0)
			failure_parse("tcheque error", lst_tok);
		if ((tmp->type == 1 || tmp->type == 2) && tmp->next->type != 0
			&& tmp->next->type != 8 && tmp->next->type != 9)
			failure_parse("tcheque error", lst_tok);
		if (tmp->type != 0 && tmp->type != 8 && tmp->type != 9
			&& (tmp->next->type == 1 || tmp->next->type == 2))
			failure_parse("tcheque error", lst_tok);
		tmp = tmp->next;
	}
	if (p != 0)
		failure_parse("tcheque error", lst_tok);
}