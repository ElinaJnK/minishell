#include "minishell.h"

void	failure_parse(const char *message, t_token *lst_tok)
{
	free_lst_tok(&lst_tok);
	perror(message);
	exit(EXIT_FAILURE);
}

char	*ft_addchr(char *s1, char c, t_token *lst_tok)
{
	size_t	s1l;
	char	*join;

	if (!s1)
		failure_parse("Error: malloc", lst_tok);
	s1l = ft_strlen(s1);
	join = (char *)malloc(sizeof(char) * (s1l + 2));
	if (!join)
		failure_parse("Error: malloc", lst_tok);
	ft_memcpy(join, s1, s1l);
	join[s1l] = c;
	join[s1l + 1] = '\0';
	if (s1)
		free(s1);
	return (join);
}

t_token	*init_tok(t_token *lst_tok)
{
	t_token	*tok;
	char	*content;

	content = (char *)malloc(sizeof(char));
	if (!content)
		return (failure_parse("Error: malloc", lst_tok), NULL);
	tok = new_token(content, 0);
	tok->content[0] = '\0';
	tok->type = 0;
	tok->next = NULL;
	return (tok);
}

t_token	*init_param(t_token **lst_tok, int *q_flag, int *i)
{
	*lst_tok = NULL;
	*q_flag = 0;
	*i = 0;
	return (init_tok(*lst_tok));
}

void	doo(t_token	**lst_tok, t_token **tok)
{
	add_back_tok(lst_tok, *tok);
	*tok = init_tok(*lst_tok);
}