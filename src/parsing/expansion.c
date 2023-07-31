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

char	*expand_env(char *line, int *i, t_env *env)
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
		while (*(line + end) && !is_op(line + end) && !is_fb(line + end)
			&& line[end] != '$' && line[end] != '\'' && line[end] != '\"'
			&& line[end] != ' ')
			end++;
		end--;
		var = search_var(line + *i + 1, end - *i, env);
	}
	newline = insert_into_line(line, var, *i, end);
	return (newline);
}

char	*expansion(char *line, int *i, t_env *env)
{
	char	*newline;
	int		end;

	end = *i;
	newline = NULL;
	if (line && line[end])
		newline = expand_env(line, &end, env);
	if (!newline)
		return (NULL);
	if (line)
		free(line);
	return (newline);
}
