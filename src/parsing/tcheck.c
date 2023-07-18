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
		if (tmp->type == OPEN_PAR)
			p++;
		if (tmp->type == CLOSE_PAR)
			p--;
		if ((tmp->next->type == OPEN_PAR && tmp->type == CMD)
			|| (tmp->type == CLOSE_PAR && tmp->next->type == CMD)
			|| ((tmp->type == AND || tmp->type == OR) && tmp->next->type != CMD
				&& tmp->next->type != OPEN_PAR && tmp->next->type != CLOSE_PAR)
			|| ((tmp->type != CMD && tmp->type != EXPAND)
				&& tmp->type != OPEN_PAR && tmp->type != CLOSE_PAR
				&& (tmp->next->type == AND || tmp->next->type == OR)))
			failure_parse("tcheque error", lst_tok);
		tmp = tmp->next;
	}
	if (p != 0)
		failure_parse("tcheque error", lst_tok);
}
