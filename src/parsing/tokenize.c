/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksadykov <ksadykov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/20 04:11:41 by ksadykov          #+#    #+#             */
/*   Updated: 2023/08/21 22:56:32 by ksadykov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token	*add_rest(t_tokyo **t)
{
	t_tokyo	*tmp;
	t_token	*tokens;

	tmp = *t;
	if (tmp->q_flag == 0 && tmp->content && tmp->content[0] != '\0')
		last_call(t);
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
	//print_list_tok(tokens);
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
		if (tmp->i > 0)
		{	
			while (tmp->i > 0 && tmp->line[tmp->i - 1] == ' ')
				(tmp->i)--;
			if (tmp->line[tmp->i - 1] == '\"' || tmp->line[tmp->i - 1] == '\'')
				add_back_tok(&(tmp->lst_tok), new_token(tmp->content, 0));
			else
				add_back_tok(&(tmp->lst_tok), new_token(tmp->content,
						get_type(tmp->content)));
		}
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
	if (tmp->line[tmp->i] && tmp->line[tmp->i + 1] != ' '
		/*&& tmp->line[tmp->i + 1] != '$'*/ && tmp->line[tmp->i + 1] != '\''
		&& tmp->line[tmp->i + 1] != '\"' && tmp->line[tmp->i + 1] != '\\'
		&& !is_op(tmp->line + tmp->i + 1) && !is_fb(tmp->line + tmp->i + 1))
	{
		tmp->line = expansionb(&tmp);
		if (!tmp->line)
			return (free_tokyo(tmp), free_lst_tok(&tmp->lst_tok), EXIT_FAILURE);
		return (EXIT_SUCCESS);
	}
	else if (*(tmp->line + tmp->i + 1) != ' ')
		tmp->i++;
	return (42);
}

int	japan(t_tokyo **t)
{
	t_tokyo	*tmp;
	int		res;

	tmp = *t;
	if (tmp->lst_tok && last_elem(tmp->lst_tok)->type == DREDIR2)
		here_doc_q(tmp->line + tmp->i, &(tmp->lst_tok));
	if (*(tmp->line + tmp->i) == '$' && tmp->q_flag != 1
		&& !is_heredoc(tmp->lst_tok))
		{
			res = to_expand(t);
			if (res == EXIT_FAILURE)
				return (EXIT_FAILURE);
			else if (res == EXIT_SUCCESS)
				return (EXIT_SUCCESS);
		}
	if ((is_op(tmp->line + tmp->i) || is_fb(tmp->line + tmp->i)
			|| tmp->line[tmp->i] == ' ') && tmp->q_flag == 0)
	{
		tmp->i += handle_op(&tmp);
		meta_tok(&tmp);
	}
	update_tok(&tmp);
	if (tmp->line[tmp->i] != '\0')
		tmp->i++;
	return (EXIT_SUCCESS);
}

t_token	*tokenize(char *line, t_env *lst_env)
{
	t_tokyo	*t;

	if (!line)
		return (NULL);
	t = init_param(line, lst_env);
	if (!t)
		return (failure_parse("Error: malloc", NULL, line), NULL);
	while (t->line && (size_t)t->i < ft_strlen(t->line) && t->line[t->i])
	{
		t->expansion = 0;
		if (japan(&t) == EXIT_FAILURE)
			return (free_tokyo(t), NULL);
	}
	return (add_rest(&t));
}
