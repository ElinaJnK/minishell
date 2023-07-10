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

	s1l = ft_strlen(s1);
	join = (char *)malloc(sizeof(char) * (s1l + 2));
	if (!join || !s1)
		failure_parse("Error: malloc", lst_tok);
	ft_memcpy(join, s1, s1l);
	join[s1l] = c;
	join[s1l + 1] = '\0';
	if (s1)
		free(s1);
	return (join);
}

void	update_tok(char *line, char **tok, int *q_flag, t_token *lst_tok)
{
	if (*line == '"' && *q_flag == 0)
			*q_flag = 2;
	else if (*line == '\'' && *q_flag == 0)
		*q_flag = 1;
	else if ((*line == '"' && *q_flag == 2)
		|| (*line == '\'' && *q_flag == 1))
		*q_flag = 0;
	else if (*line != ' ' && !is_op(line) && !is_fb(line))
		*tok = ft_addchr(*tok, *line, lst_tok);
}

void	init_tok(char **tok, t_token *lst_tok)
{
	*tok = (char *)malloc(sizeof(char));
	if (!*tok)
		return (failure_parse("Error: malloc", lst_tok));
	*tok[0] = '\0';
}

void	add_rest(t_token **lst_tok, char *tok, int q_flag)
{
	if (q_flag == 0 && tok != NULL && tok[0] != '\0')
		add_back_tok(lst_tok, new_token(tok, get_type(tok)));
	else if (q_flag == 1 || q_flag == 2)
	{
		free(tok);
		failure_parse("Error: unclosed quote", *lst_tok);
	}
}

void	meta_tok(char *line, int *i, t_token **lst_tok)
{
	char	*tok;

	tok = malloc(sizeof(t_token));
	if (!tok)
		failure_parse("Error: malloc", *lst_tok);
	if (is_op(line + *i) || is_fb(line + *i) == 6 || is_fb(line + *i) == 7)
	{
		tok = ft_substr(line, *i, 2);
		add_back_tok(lst_tok, new_token(tok, get_type(tok)));
		*i += 2;
	}
	else if (is_fb(line + *i))
	{
		tok = ft_addchr(tok, line[*i], *lst_tok);
		add_back_tok(lst_tok, new_token(tok, get_type(tok)));
		*i += 1;
	}
}

void	init_param(t_token **lst_tok, char **tok, int *q_flag, int *i)
{
	*lst_tok = NULL;
	*q_flag = 0;
	*i = 0;
	init_tok(tok, *lst_tok);
}

t_token	*tokenize(char *line)
{
	t_token	*lst_tok;
	char	*tok;
	int		q_flag;
	int		i;

	init_param(&lst_tok, &tok, &q_flag, &i);
	while ((size_t)i < ft_strlen(line))
	{
		if (is_op(line + i) || is_fb(line + i) || line[i] == ' ')
		{
			if (q_flag == 0 && tok != NULL && tok[0] != '\0')
			{
				add_back_tok(&lst_tok, new_token(tok, get_type(tok)));
				init_tok(&tok, lst_tok);
			}
			else if (q_flag == 1 || q_flag == 2)
				tok = ft_addchr(tok, line[i], lst_tok);
			meta_tok(line, &i, &lst_tok);
		}
		update_tok(line + i, &tok, &q_flag, lst_tok);
		if (line[i] != '\0' && !is_op(line + i) && !is_fb(line + i))
			i++;
	}
	add_rest(&lst_tok, tok, q_flag);
	return (check_tok(lst_tok), lst_tok);
}

/*--------------------------------TEST-------------------------------------*/
// void	print_list_tok(t_token *lst_tok)
// {
// 	t_token	*temp;

// 	temp = lst_tok;
// 	while (temp)
// 	{
// 		// printf("[%s, %d]->", temp->content, temp->type);
// 		printf("[%s]->", temp->content);
// 		temp = temp->next;
// 	}
// 	printf("\n");
// }

// int	main(void)
// {
// 	t_token	*lst_tok;

// 	// lst_tok = tokenize("ech\'o\' hello&&(echo \"world\" || echo \"hello world\")");
// 	//lst_tok = tokenize("ech\'o\' hello&&(echo \"world\" || echo \"hello world\")");
// 	lst_tok = tokenize("(((echo bye)) && cd dossier||echo he'l'lo ) | cat -e");	
// 	// ( smpl_cmd && smpl_cmd || smpl_cmd ) | smpl_cmd
// 	printf("Testing 1 2 1 2, the mic is on\n");
// 	print_list_tok(lst_tok);
// 	return (0);
// }
/*--------------------------------------------------------------------------*/