#include "minishell.h"

char	*insert_into_line(char *line, char *var, int start, int end)
{
	char	*newline;

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

char	*expand_env(char *line, int *i)
{
	char	*var;
	char	*newline;
	int		end;

	end = *i;
	while (line && !is_op(line[end]) && !is_sep(line[end]))
		end++;
	var = getenv(ft_memmove(line + *i, end - *i));
	if (!var)
		return (NULL);
	newline = insert_into_line(line, var, *i, end);
	*i += ft_strlen(var);
	free(var);
	return (newline);
}

char	*expansion(char *line, int *i)
{
	char	*newline;
	int		end;

	end = *i + 1;
	newline = NULL;
	if (line && line[end])
		newline = expand_env(line, &end);
	else if (!newline)
		failure("Error in expansion");
	*i = end;
	return (newline);
}
