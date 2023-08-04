#include "minishell.h"

void	add_rest(t_tokyo **t)
{
	t_tokyo	*tmp;

	tmp = *t;
	if (tmp->q_flag == 0 && tmp->content && tmp->content[0] != '\0')
		add_back_tok(&(tmp->lst_tok), new_token(tmp->content, 0));
	else if (tmp->q_flag == 1 || tmp->q_flag == 2)
	{
		if (tmp->content)
		{
			free(tmp->content);
			tmp->content = NULL;
		}
		ft_putstr_fd("bash: unclosed quotes\n", STDIN_FILENO);
		free_lst_tok(&(tmp->lst_tok));
	}
}

void	meta_tok(t_tokyo **t)
{
	char	*tok;
	t_tokyo	*tmp;

	tmp = *t;
	tok = malloc(sizeof(char));
	if (!tok)
		failure_parse("Error: malloc", tmp->lst_tok, tmp->line);
	*tok = '\0';
	if (is_op(tmp->line + tmp->i) || is_fb(tmp->line + tmp->i) == 6
		|| is_fb(tmp->line + tmp->i) == 7)
	{ 
		free(tok);
		tok = ft_substr(tmp->line, tmp->i, 2);
		add_back_tok(&(tmp->lst_tok), new_token(tok, get_type(tok)));
		tmp->i += 2;
	}
	else if (is_fb(tmp->line + tmp->i))
	{
		tok = ft_addchr(tok, tmp->line[tmp->i], tmp->lst_tok, tmp->line);
		add_back_tok(&(tmp->lst_tok), new_token(tok, get_type(tok)));
		tmp->i += 1;
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
		if (is_op(line) || is_fb(line) == DREDIR
			|| is_fb(line) == DREDIR2 || is_fb(line) == DREDIR2_E)
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

int	handle_op(t_tokyo **tokyo)
{
	int		i;
	t_tokyo	*tmp;

	i = 0;
	tmp = *tokyo;
	if (tmp->q_flag == 0 && tmp->content != NULL && tmp->content[0] != '\0')
	{
		add_back_tok(&(tmp->lst_tok), new_token(tmp->content, 0));
		tmp->content = NULL;
	}
	else if (tmp->q_flag == 1 || tmp->q_flag == 2)
		i = is_true_op(tmp->line + tmp->i, &(tmp->content), &(tmp->lst_tok));
	return (i);
}
/*
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
}*/


int	next_to_expand(char *line)
{
	if (line[0] && !is_op(line) && !is_fb(line)
		&& line[0] != '$' && line[0] != '\'' && line[0] != '\"'
		&& line[0] != ' ')
		return (1);
	return (0);
}

// t_token	*tokenize(char *line, t_env *lst_env)
// {
// 	t_token	*tok;
// 	int		q_flag;
// 	int		i;
// 	char	*content;
// 	int		exp;

// 	exp = 0;
// 	init_param(&content, &tok, &q_flag, &i);
// 	while ((size_t)i < ft_strlen(line) && line[i])
// 	{
// 		exp = 0;
// 		if (((line[i] == '\'' && line[i + 1] == '\'')
// 			|| (line[i] == '"' && line[i + 1] == '"'))
// 			&& q_flag == 0)
// 		{
// 			add_back_tok(&tok, new_token(ft_strdup(""), 0));
// 			i += 2;
// 			continue ;
// 		}
// 		if (tok && last_elem(tok)->type == DREDIR2)
// 			here_doc_q(line + i, &tok);
// 		if (*(line + i) == '$' && q_flag != 1 && !is_heredoc(tok))
// 		{
// 			if (next_to_expand(line + i + 1))
// 			{
// 				line = expansion(line, &i, lst_env, tok);
// 				exp = 1;
// 				if (!line)
// 					return (NULL);
// 			}
// 			else
// 				i++;
// 		}
// 		if (is_op(line + i) || is_fb(line + i) || line[i] == ' ')
// 		{
// 			i += handle_op(q_flag, line + i, &content, &tok);
// 			meta_tok(line, &i, &tok);
// 		}
// 		update_tok(line + i, &content, &q_flag, tok, exp);
// 		if (line[i] != '\0' && !is_op(line + i) && !is_fb(line + i))
// 			i++;
// 	}
// 	return (add_rest(&tok, content, q_flag), check_tok(&tok),
// 		free(line), tok);
// }

int	japan(t_tokyo **t)
{
	t_tokyo	*tmp;

	tmp = *t;
	if (tmp->lst_tok && last_elem(tmp->lst_tok)->type == DREDIR2)
		here_doc_q(tmp->line + tmp->i, &(tmp->lst_tok));
	if (*(tmp->line + tmp->i) == '$' && tmp->q_flag != 1 && !is_heredoc(tmp->lst_tok))
	{
		if (next_to_expand(tmp->line + tmp->i + 1))
		{
			tmp->line = expansion(tmp->line, &(tmp->i), tmp->lst_env, tmp->lst_tok);
			tmp->expansion = 1;
			if (!tmp->line)
				return (free_tokyo(tmp), EXIT_FAILURE);
		}
		else if (*(tmp->line + tmp->i + 1) != ' ')
			tmp->i++;
	}
	if (is_op(tmp->line + tmp->i) || is_fb(tmp->line + tmp->i) || tmp->line[tmp->i] == ' ')
	{
		tmp->i += handle_op(&tmp);
		meta_tok(&tmp);
	}
	update_tok(&tmp);
	if (tmp->line[tmp->i] != '\0' && !is_op(tmp->line + tmp->i) && !is_fb(tmp->line + tmp->i))
		tmp->i++;
	return (EXIT_SUCCESS);
}

t_token	*tokenize(char *line, t_env *lst_env)
{
	t_tokyo	*t;
	t_token	*tokens;

	tokens = NULL;
	t = init_param(line, lst_env);
	if (!t)
		return (failure_parse("Error: malloc", NULL, line), NULL);
	while ((size_t)t->i < ft_strlen(t->line) && t->line[t->i])
	{
		t->expansion = 0;
		if (((t->line[t->i] == '\'' && t->line[t->i + 1] == '\'')
			|| (t->line[t->i] == '"' && t->line[t->i + 1] == '"'))
			&& t->q_flag == 0)
		{
			add_back_tok(&(t->lst_tok), new_token(ft_strdup(""), 0));
			t->i += 2;
			continue ;
		}
		if (japan(&t) == EXIT_FAILURE)
			return (free_tokyo(t), NULL);
	}
	add_rest(&t);
	check_tok(&(t->lst_tok));
	tokens = t->lst_tok;
	free_tokyo(t);
	return (tokens);
}