#include "minishell.h"

void	failure_parse(const char *message, t_token *lst_tok, char *line)
{
	free_lst_tok(&lst_tok);
	if (line)
		free(line);
	perror(message);
	exit(EXIT_FAILURE);
}

char	*ft_addchr(char *s1, char c, t_token *lst_tok, char *line)
{
	size_t	s1l;
	char	*join;

	if (!s1)
		s1l = 0;
	else
	{
		s1l = ft_strlen(s1);
	}
	join = (char *)malloc(sizeof(char) * (s1l + 2));
	if (!join)
		failure_parse("Error: malloc", lst_tok, line);
	if (s1)
		ft_memcpy(join, s1, s1l);
	join[s1l] = c;
	join[s1l + 1] = '\0';
	if (s1)
		free(s1);
	return (join);
}

void	update_tok(char *line, char **content, int *q_flag, t_token *lst_tok)
{
	if (*line == '"' && *q_flag == 0)
		*q_flag = 2;
	else if (*line == '\'' && *q_flag == 0)
		*q_flag = 1;
	else if ((*line == '"' && *q_flag == 2)
		|| (*line == '\'' && *q_flag == 1))
		*q_flag = 0;
	else if (*line != ' ' && !is_op(line) && !is_fb(line) && *line != '\0')
		*content = ft_addchr(*content, *line, lst_tok, line);
}

void	init_param(char **content, t_token **lst_tok, int *q_flag, int *i)
{
	*content = NULL;
	*lst_tok = NULL;
	*q_flag = 0;
	*i = 0;
}

