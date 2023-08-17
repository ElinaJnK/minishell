#include "minishell.h"

int		raccourci(t_tokyo **t, char *var)
{
	int	len;
	int	i;

	i = 0;
	len = 0;
	if (var)
		len = ft_strlen(var);
	while (i < len)
	{
		(*t)->content = ft_addchr((*t)->content, var[i],
				(*t)->lst_tok, (*t)->line);
		i++;
	}
	return (i);
}

char	*expand_envb(t_tokyo **t)
{
	char	*var;
	char	*newline;
	int		end;
	int		ret;

	end = (*t)->i + 1;
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
			&& !is_fb((*t)->line + end) && (*t)->line[end] != '$'
			&& (*t)->line[end] != '\'' && (*t)->line[end] != '\"'
			&& (*t)->line[end] != ' ' && (*t)->line[end] != '\n')
			end++;
		end--;
		var = search_var((*t)->line + (*t)->i + 1, end - (*t)->i,
			(*t)->lst_env);
	}
	ret = raccourci(t, var);
	newline = insert_into_line((*t)->line, var, (*t)->i, end);
	(*t)->i += ret;
	return (newline);
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
