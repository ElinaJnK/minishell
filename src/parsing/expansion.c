#include "minishell.h"

char	*insert_into_line(char *line, char *var, int start, int end)
{
	char	*newline;
	char	*one;
	char	*two;
	char	*three;

	newline = malloc(sizeof(char) * (ft_strlen(line)
				- (end - start) + ft_strlen(var) + 1));
	if (!newline)
		return (NULL);
	one = ft_substr(line, 0, start);
	two = ft_strjoin(one, var);
	three = ft_substr(line, end + 1, ft_strlen(line) - end);
	newline = ft_strjoin(two, three);
	//free(line);
	free(one);
	free(two);
	free(three);
	free(var);
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
	return (ft_strdup(""));
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
	else if (!newline)
		return (NULL);
	return (newline);
}
