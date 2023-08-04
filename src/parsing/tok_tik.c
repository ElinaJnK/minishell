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
		s1l = ft_strlen(s1);
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

void	update_tok(t_tokyo **t)
{
	t_tokyo	*tmp;

	tmp = *t;
	if (tmp->expansion == 0 && *(tmp->line + tmp->i) == '"' && tmp->q_flag == 0)
		tmp->q_flag = 2;
	else if (tmp->expansion == 0 && *(tmp->line + tmp->i) == '\'' && tmp->q_flag == 0)
		tmp->q_flag = 1;
	else if (tmp->expansion == 0 && ((*(tmp->line + tmp->i) == '"' && tmp->q_flag == 2)
		|| (*(tmp->line + tmp->i) == '\'' && tmp->q_flag == 1)))
		tmp->q_flag = 0;
	else if ((tmp->expansion == 0 && *(tmp->line + tmp->i) != ' '
			&& !is_op(tmp->line + tmp->i) && !is_fb(tmp->line + tmp->i)
			&& *(tmp->line + tmp->i) != '\0')
			|| tmp->expansion == 1)
		tmp->content = ft_addchr(tmp->content, *(tmp->line + tmp->i), tmp->lst_tok, tmp->line);
}

t_tokyo	*init_param(char *line, t_env *lst_env)
{
	t_tokyo	*t;

	t = malloc(sizeof(t_tokyo));
	if (!t)
		return (NULL);
	t->content = NULL;
	t->lst_tok = NULL;
	t->q_flag = 0;
	t->i = 0;
	t->line = line;
	t->expansion = 0;
	t->lst_env = lst_env;
	return (t);
}

void	free_tokyo(t_tokyo *t)
{
	if (t)
	{
		if (t->line)
			free(t->line);
		free(t);
	}
}