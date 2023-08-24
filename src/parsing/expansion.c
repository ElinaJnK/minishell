/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ejankovs <ejankovs@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/20 04:08:40 by ksadykov          #+#    #+#             */
/*   Updated: 2023/08/24 18:59:41 by ejankovs         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*insert_into_line(char *line, char *var, int start, int end)
{
	char	*newline;
	char	*one;
	char	*two;
	char	*three;

	one = ft_substr(line, 0, start);
	if (var)
		two = ft_strjoin(one, var);
	three = ft_substr(line, end + 1, ft_strlen(line) - end);
	if (var)
	{
		newline = ft_strjoin(two, three);
		free(two);
		free(var);
	}
	else
		newline = ft_strjoin(one, three);
	free(one);
	free(three);
	return (newline);
}

char	*search_var(char *var, int size, t_env *env)
{
	t_env	*tmp;

	tmp = env;
	while (tmp)
	{
		if (ft_strncmp(tmp->name, var, size) == 0
			&& ft_strlen(tmp->name) == (size_t)size)
			return (ft_strdup(tmp->value));
		tmp = tmp->next;
	}
	return (NULL);
}

char	*expand_env(char *line, int *i, t_env *env, int *e)
{
	char	*var;
	char	*newline;
	int		end;

	end = *i + 1;
	if (*(line + *i + 1) == '?')
	{
		end = *i + 1;
		var = ft_itoa(*exit_status());
		if (!var)
			return (NULL);
	}
	else
	{
		while (line[end] && !is_op(line + end) && !is_fb(line + end)
			&& line[end] != '$' && line[end] != '\'' && line[end] != '\"'
			&& !is_whitespace(line[end]) && line[end] != '\n')
			end++;
		end--;
		var = search_var(line + *i + 1, end - *i, env);
		if (var && (!ft_strncmp(var, "\"", 2) || !ft_strncmp(var, "\'", 2)))
			*e = 1;
	}
	newline = insert_into_line(line, var, *i, end);
	return (newline);
}

char	*expansion(t_tokyo *t)
{
	char	*newline;

	newline = NULL;
	if (t->line && t->line[t->i])
		newline = expand_env(t->line, &(t->i), t->lst_env, &(t->expansion));
	if (!newline)
		return (failure_parse("malloc error", t->lst_tok, t->line), NULL);
	if (t->line)
		free(t->line);
	return (newline);
}

char	*expansion_here(char *line, int *i, t_env *lst_env, t_token *lst_tok)
{
	char	*newline;
	int		myst;

	myst = 0;
	newline = NULL;
	if (line && line[*i])
		newline = expand_env(line, i, lst_env, &myst);
	if (!newline)
		return (failure_parse("malloc error", lst_tok, line), NULL);
	if (line)
		free(line);
	return (newline);
}
