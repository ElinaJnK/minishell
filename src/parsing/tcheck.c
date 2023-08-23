/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tcheck.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ejankovs <ejankovs@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/20 04:08:10 by ksadykov          #+#    #+#             */
/*   Updated: 2023/08/23 11:51:32 by ejankovs         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	put_error_tok(char *message, t_token **lst_tok)
{
	ft_putstr_fd(message, 2);
	*(exit_status()) = 2;
	if (lst_tok)
		free_lst_tok(lst_tok);
	lst_tok = NULL;
}

int	check_first_tok(t_token *tmp, t_token **lst_tok)
{
	if (((tmp->type >= 1 && tmp->type <= 3) || tmp->type == CLOSE_PAR))
		return (put_error_tok("bash: syntax error near unexpected token\n",
				lst_tok), EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

int	check_last_tok(t_token *tok, int p, t_token **lst_tok)
{
	if ((tok->type >= 1 && tok->type <= 3) || tok->type == OPEN_PAR
		|| tok->type == REDIR || tok->type == REDIR2
		|| tok->type == DREDIR || tok->type == DREDIR2)
		return (put_error_tok("bash: syntax error near unexpected token\n",
				lst_tok), EXIT_FAILURE);
	if (tok->type == CLOSE_PAR)
		p--;
	if (p != 0)
		return (put_error_tok("bash: syntax error near unexpected token\n",
				lst_tok), EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

int	verif(t_token *tmp, int *p)
{
	if (tmp->type == OPEN_PAR)
		*p += 1;
	else if (tmp->type == CLOSE_PAR)
		*p -= 1;
	if (tmp->next)
	{
		if ((tmp->next->type == OPEN_PAR && tmp->type == CMD)
			|| (tmp->type == CLOSE_PAR && tmp->next->type == CMD))
			return (1);
		if (tmp->type >= 1 && tmp->type <= 3
			&& tmp->next->type >= 1 && tmp->next->type <= 3)
			return (1);
		if (tmp->type == tmp->next->type && tmp->type >= 1 && tmp->type <= 8
			&& tmp->type >= 1 && tmp->type <= 8)
			return (1);
		if (tmp->type >= REDIR && tmp->type <= DREDIR2_E
			&& (tmp->next->type == OPEN_PAR || tmp->next->type == CLOSE_PAR))
			return (1);
	}
	return (0);
}

void	check_tok(t_token **lst_tok)
{
	t_token	*tmp;
	int		p;

	tmp = *lst_tok;
	p = 0;
	if (!tmp)
		return ;
	if (check_first_tok(tmp, lst_tok) == EXIT_FAILURE)
		return ;
	while (tmp && tmp->next)
	{
		if (verif(tmp, &p))
		{
			put_error_tok("bash: syntax error near unexpected token\n", lst_tok);
			return ;
		}
		tmp = tmp->next;
	}
	if (check_last_tok(tmp, p, lst_tok) == EXIT_FAILURE)
		return ;
}
