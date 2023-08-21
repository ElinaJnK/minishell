/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tok_type.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ejankovs <ejankovs@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/20 20:05:47 by ksadykov          #+#    #+#             */
/*   Updated: 2023/08/20 20:05:47 by ejankovs         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
	else if (ft_strncmp(line, ">>", 2) == 0)
		return (DREDIR);
	else if (ft_strncmp(line, "<<", 2) == 0)
		return (DREDIR2);
	else if (ft_strncmp(line, ">", 1) == 0)
		return (REDIR);
	else if (ft_strncmp(line, "<", 1) == 0)
		return (REDIR2);
	else if (ft_strncmp(line, "(", 1) == 0)
		return (OPEN_PAR);
	else if (ft_strncmp(line, ")", 1) == 0)
		return (CLOSE_PAR);
	return (CMD);
}

void	meta_tok(t_tokyo **t)
{
	char	*tok;
	t_tokyo	*tmp;

	tmp = *t;
	tok = malloc(sizeof(char));
	if (!tok)
		failure_parse("Error: malloc", tmp->lst_tok, tmp->line);
	*tok = '\0';
	if (is_op(tmp->line + tmp->i) || is_fb(tmp->line + tmp->i) == 6
		|| is_fb(tmp->line + tmp->i) == 7)
	{
		free(tok);
		tok = ft_substr(tmp->line, tmp->i, 2);
		add_back_tok(&(tmp->lst_tok), new_token(tok, get_type(tok)));
		tmp->i += 2;
	}
	else if (is_fb(tmp->line + tmp->i))
	{
		tok = ft_addchr(tok, tmp->line[tmp->i], tmp->lst_tok, tmp->line);
		add_back_tok(&(tmp->lst_tok), new_token(tok, get_type(tok)));
		tmp->i += 1;
	}
	else
		free(tok);
}

int	is_true_op(char *line, char **content, t_token **lst_tok)
{
	int	i;

	i = 0;
	if (is_op(line) || is_fb(line))
	{
		if (is_op(line) || is_fb(line) == DREDIR
			|| is_fb(line) == DREDIR2 || is_fb(line) == DREDIR2_E)
		{
			*content = ft_addchr(*content, *line, *lst_tok, line);
			i++;
			*content = ft_addchr(*content, *(line + 1), *lst_tok, line);
			i++;
		}
		else
		{
			*content = ft_addchr(*content, *line, *lst_tok, line);
			i = 1;
		}
	}
	else
		*content = ft_addchr(*content, *line, *lst_tok, line);
	return (i);
}
