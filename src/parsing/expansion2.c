#include "minishell.h"

int	expand_env_bis(char *line, char *content, t_env *env, t_token *lst_tok)
{
	char	*var;
	int		end;
	int		i;

	end = 1;
	i = 0;
	if (*(line + 1) == '?')
	{
		end++;
		var = ft_itoa(*exit_status());
		if (!var)
			return (-1);
	}
	else
	{
		while (line[end] && !is_op(line + end) && !is_fb(line + end)
			&& line[end] != '$' && line[end] != '\'' && line[end] != '\"'
			&& line[end] != ' ')
			end++;
		end--;
		var = search_var(line + 1, end, env);
		printf("var expanded : %s\n", var);
	}
	while ((size_t)i < ft_strlen(var))
	{
		content = ft_addchr(content, var[i], lst_tok, line);
		i++;
	}
	printf("newcontent : %s\n", content);
	printf("end : %d\n", end);
	if (var)
		free(var);
	return (end + 1);
}

int	expansion_bis(char *line, char **content, t_env *env, t_token *lst_tok)
{
	if (line && *line)
		return (expand_env_bis(line, *content, env, lst_tok));
	return (-1);
}
