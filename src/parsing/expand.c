/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ejankovs <ejankovs@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/20 04:14:08 by ksadykov          #+#    #+#             */
/*   Updated: 2023/08/20 04:14:3 by ejankovs         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	raccourci(t_tokyo **t, char *var)
{
	int	len;
	int	i;

	i = 0;
	len = 0;
	if (!var)
		return (0);
	len = ft_strlen(var);
	while (i < len)
	{
		if (is_whitespace(var[i]) && (*t)->q_flag != 0)
			(*t)->content = ft_addchr((*t)->content, var[i],
					(*t)->lst_tok, (*t)->line);
		else if (!is_whitespace(var[i]))
			(*t)->content = ft_addchr((*t)->content, var[i],
					(*t)->lst_tok, (*t)->line);
		if (is_whitespace(var[i]) && (*t)->q_flag == 0 && (*t)->content != NULL
			&& (*t)->content[0] != '\0')
		{
			add_back_tok(&((*t)->lst_tok), new_token((*t)->content, 0));
			(*t)->content = NULL;
		}
		i++;
	}
	return (i);
}

int	add_var(t_tokyo **t, char *var, int end)
{
	int	i;

	i = 0;
	while (i < end)
	{
		(*t)->content = ft_addchr((*t)->content, var[i],
				(*t)->lst_tok, (*t)->line);
		i++;
	}
	return (i);
}

char	*expand_while(t_tokyo **t, int end)
{
	char	*var;

	if (check_name_bis((*t)->line + (*t)->i + 1,
			end - (*t)->i) == EXIT_FAILURE)
	{
		var = NULL;
		(*t)->i += add_var(t, (*t)->line + (*t)->i, end - (*t)->i + 1);
	}
	else
		var = search_var((*t)->line + (*t)->i + 1, end - (*t)->i,
				(*t)->lst_env);
	return (var);
}

char	*expand_envb(t_tokyo **t)
{
	char	*var;
	char	*newline;
	int		end;
	int		res;

	end = (*t)->i + 1;
	res = 0;
	if (*((*t)->line + (*t)->i + 1) == '?')
	{
		end = (*t)->i + 1;
		var = ft_itoa(*exit_status());
		if (!var)
			return (NULL);
	}
	else
	{
		while ((*t)->line[end] && !is_op((*t)->line + end)
			&& !is_fb((*t)->line + end) && check_special_carac((*t)->line[end]))
			end++;
		end--;
		var = expand_while(t, end);
	}
	res += raccourci(t, var);
	newline = insert_into_line((*t)->line, var, (*t)->i, end);
	return ((*t)->i += res, newline);
}

char	*expansionb(t_tokyo **t)
{
	char	*newline;

	newline = NULL;
	if ((*t)->line && (*t)->line[(*t)->i])
		newline = expand_envb(t);
	if (!newline)
		return (failure_parse("malloc error", (*t)->lst_tok, (*t)->line), NULL);
	if ((*t)->line)
		free((*t)->line);
	return (newline);
}
