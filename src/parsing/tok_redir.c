/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tok_redir.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksadykov <ksadykov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/21 06:11:12 by ksadykov          #+#    #+#             */
/*   Updated: 2023/08/21 21:14:59 by ksadykov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	merge(t_token **res, t_token *cmd, t_token *redirs)
{
	t_token	*tmp;

	tmp = NULL;
	if (!*res)
	{
		*res = cmd;
		tmp = last_elem(*res);
		if (!tmp)
			*res = redirs;
		else
			tmp->next = redirs;
		return ;
	}
	tmp = last_elem(*res);
	tmp->next = cmd;
	tmp = last_elem(*res);
	tmp->next = redirs;
}

void	restructure_tok(t_token **tmp, t_token **cmds, t_token **redirs,
		t_token **res)
{
	if ((*tmp)->type == CMD)
		add_back_tok(cmds, new_token(ft_strdup((*tmp)->content), (*tmp)->type));
	else if ((*tmp)->type >= REDIR && (*tmp)->type <= DREDIR2_E && (*tmp)->next)
	{
		add_back_tok(redirs, new_token(ft_strdup((*tmp)->next->content),
				(*tmp)->type));
		*tmp = (*tmp)->next;
	}
	else
	{
		merge(res, *cmds, *redirs);
		*cmds = NULL;
		*redirs = NULL;
		add_back_tok(res, new_token(ft_strdup((*tmp)->content), (*tmp)->type));
	}
}

t_token	*tokenize_bise(t_token *tok)
{
	t_token	*cmds;
	t_token	*redirs;
	t_token	*res;
	t_token	*tmp;

	tmp = tok;
	res = NULL;
	cmds = NULL;
	redirs = NULL;
	if (!tok)
		return (NULL);
	while (tmp)
	{
		restructure_tok(&tmp, &cmds, &redirs, &res);
		tmp = tmp->next;
	}
	merge(&res, cmds, redirs);
	cmds = NULL;
	redirs = NULL;
	if (tok)
		free_lst_tok(&tok);
	return (res);
}

void	last_call(t_tokyo **t)
{
	t_tokyo	*tmp;

	tmp = *t;
	if (tmp->i > 0)
	{
		while (tmp->i > 0 && tmp->line[tmp->i - 1] == ' ')
			(tmp->i)--;
		if (tmp->line[tmp->i - 1] == '\"' || tmp->line[tmp->i - 1] == '\'')
			add_back_tok(&(tmp->lst_tok), new_token(tmp->content, 0));
		else
			add_back_tok(&(tmp->lst_tok), new_token(tmp->content,
					get_type(tmp->content)));
	}
}
