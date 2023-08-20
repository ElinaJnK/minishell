/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tok_star.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksadykov <ksadykov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/20 04:08:24 by ksadykov          #+#    #+#             */
/*   Updated: 2023/08/20 05:23:43 by ksadykov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

char	*find_pwd(t_env *lst_env)
{
	t_env	*tmp;

	tmp = lst_env;
	while (tmp)
	{
		if (!ft_strncmp(tmp->name, "PWD", 4))
			return (tmp->value);
		tmp = tmp->next;
	}
	return (NULL);
}

t_token	*tokenize_crise(t_token *tok, t_env *lst_env)
{
	t_token	*tmp;
	char	*pwd;
	t_token	*res;
	int		flag;

	if (!tok)
		return (NULL);
	pwd = find_pwd(lst_env);
	if (!pwd)
		return (failure("bash : pwd"), NULL);
	tmp = tok;
	flag = 0;
	res = NULL;
	while (tmp)
		check_wildcard(&tmp, &res, &flag, pwd);
	if (tok)
		free_lst_tok(&tok);
	return (res);
}
