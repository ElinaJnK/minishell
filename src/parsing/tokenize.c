#include "minishell.h"

t_token	*add_rest(t_tokyo **t)
{
	t_tokyo	*tmp;
	t_token	*tokens;

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
		ft_putstr_fd("bash: unclosed quotes\n", STDERR_FILENO);
		return (free_lst_tok(&(tmp->lst_tok)), free_tokyo(*t), NULL);
	}
	check_tok(&((*t)->lst_tok));
	tokens = (*t)->lst_tok;
	return (free_tokyo(*t), tokens);
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

int	to_expand(t_tokyo **t)
{
	t_tokyo	*tmp;

	tmp = *t;
	if (tmp->line[tmp->i + 1] && tmp->line[tmp->i + 1] != ' '
		&& tmp->line[tmp->i + 1] != '$' && tmp->line[tmp->i + 1] != '\''
		&& tmp->line[tmp->i + 1] != '\"' && tmp->line[tmp->i + 1] != '\\'
		&& !is_op(tmp->line + tmp->i + 1) && !is_fb(tmp->line + tmp->i + 1))
	{
		tmp->line = expansionb(&tmp);
		if (!tmp->line)
			return (free_tokyo(tmp), free_lst_tok(&tmp->lst_tok), EXIT_FAILURE);
	}
	else if (*(tmp->line + tmp->i + 1) != ' ')
		tmp->i++;
	return (EXIT_SUCCESS);
}

int	japan(t_tokyo **t)
{
	t_tokyo	*tmp;

	tmp = *t;
	if (tmp->lst_tok && last_elem(tmp->lst_tok)->type == DREDIR2)
		here_doc_q(tmp->line + tmp->i, &(tmp->lst_tok));
	if (*(tmp->line + tmp->i) == '$' && tmp->q_flag != 1
		&& !is_heredoc(tmp->lst_tok) && to_expand(&tmp) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (is_op(tmp->line + tmp->i) || is_fb(tmp->line + tmp->i)
		|| tmp->line[tmp->i] == ' ')
	{
		tmp->i += handle_op(&tmp);
		meta_tok(&tmp);
	}
	update_tok(&tmp);
	if (tmp->line[tmp->i] != '\0' && !is_op(tmp->line + tmp->i)
		&& !is_fb(tmp->line + tmp->i))
		tmp->i++;
	return (EXIT_SUCCESS);
}

t_token	*tokenize(char *line, t_env *lst_env)
{
	t_tokyo	*t;

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
	return (add_rest(&t));
}
