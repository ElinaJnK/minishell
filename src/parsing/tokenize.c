#include "minishell.h"

void	add_rest(t_token **lst_tok, char *content, int q_flag)
{
	if (q_flag == 0 && content && content[0] != '\0')
		add_back_tok(lst_tok, new_token(content, 0));
	else if (q_flag == 1 || q_flag == 2)
	{
		free(content);
		ft_putstr_fd("bash: unclosed quotes\n", STDIN_FILENO);
		free_lst_tok(lst_tok);
	}
	print_list_tok(*lst_tok);
}

void	meta_tok(char *line, int *i, t_token **lst_tok)
{
	char	*tok;

	tok = malloc(sizeof(char));
	if (!tok)
		failure_parse("Error: malloc", *lst_tok, line);
	*tok = 0;
	if (is_op(line + *i) || is_fb(line + *i) == 6 || is_fb(line + *i) == 7)
	{
		free(tok);
		tok = ft_substr(line, *i, 2);
		add_back_tok(lst_tok, new_token(tok, get_type(tok)));
		*i += 2;
	}
	else if (is_fb(line + *i))
	{
		tok = ft_addchr(tok, line[*i], *lst_tok, line);
		add_back_tok(lst_tok, new_token(tok, get_type(tok)));
		*i += 1;
	}
	else
		free(tok);
}

int	is_true_op(char *line, char **content, t_token **lst_tok)
{
	int	i;

	i = 0;
	if (is_op(line) || is_fb(line))
	{
		if (is_op(line) || is_fb(line) == DREDIR2
			|| is_fb(line) == DREDIR2)
		{
			*content = ft_addchr(*content, *line, *lst_tok, line);
			i++;
			*content = ft_addchr(*content, *(line + 1), *lst_tok, line);
			i++;
		}
		else
		{
			*content = ft_addchr(*content, *line, *lst_tok, line);
			i = 1;
		}
	}
	else
		*content = ft_addchr(*content, *line, *lst_tok, line);
	return (i);
}

int	handle_op(int q_flag, char *line, char **content, t_token **lst_tok)
{
	int	i;

	i = 0;
	if (q_flag == 0 && *content != NULL && *content[0] != '\0')
	{
		add_back_tok(lst_tok, new_token(*content, 0));
		*content = NULL;
	}
	else if (q_flag == 1 || q_flag == 2)
		i = is_true_op(line + i, content, lst_tok);
	return (i);
}

t_token	*tokenize(char *line, t_env *lst_env)
{
	t_token	*tok;
	int		q_flag;
	int		i;
	char	*content;

	init_param(&content, &tok, &q_flag, &i);
	while ((size_t)i < ft_strlen(line) && line[i])
	{
		if (tok && last_elem(tok)->type == DREDIR2)
			here_doc_q(line + i, &tok);
		if (*(line + i) == '$' && q_flag != 1 && !is_heredoc(tok))
			line = expansion(line, &i, lst_env, tok);
		else if (is_op(line + i) || is_fb(line + i) || line[i] == ' ')
		{
			i += handle_op(q_flag, line + i, &content, &tok);
			meta_tok(line, &i, &tok);
		}
		update_tok(line + i, &content, &q_flag, tok);
		if (line[i] != '\0' && !is_op(line + i) && !is_fb(line + i))
			i++;
	}
	return (add_rest(&tok, content, q_flag), check_tok(&tok),
		free(line), tok);
}
