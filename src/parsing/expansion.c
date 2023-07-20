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
	three = ft_substr(line, end, ft_strlen(line) - end);
	newline = ft_strjoin(two, three);
	free(line);
	free(one);
	free(two);
	free(three);
	return (newline);
}

char	*search_var(char *var, t_env *env)
{
	t_env	*tmp;

	tmp = env;
	while (tmp)
	{
		if (strcmp(var, tmp->name))
			return (tmp->value);
		tmp = tmp->next;
	}
	return (NULL);
}

char	*expand_env(char *line, int *i, t_env *env)
{
	char	*var;
	char	*name;
	char	*newline;
	int		end;

	end = *i;
	while (line && !is_op(line + end) && !is_fb(line + end))
		end++;
	name = malloc(sizeof(char) * (end - *i + 1));
	name[end - *i] = '\0';
	if (!name)
		return (NULL);
	var = search_var(ft_memmove(name, line + *i, end - *i), env);
	if (!var)
		return (NULL);
	newline = insert_into_line(line, var, *i, end);
	*i += ft_strlen(var);
	free(var);
	return (newline);
}

char	*expansion(char *line, int *i, t_env *env)
{
	char	*newline;
	int		end;

	end = *i + 1;
	newline = NULL;
	if (line && line[end])
		newline = expand_env(line, &end, env);
	else if (!newline)
		failure("Error in expansion");
	*i = end;
	return (newline);
}
