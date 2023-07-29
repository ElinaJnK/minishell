#include "minishell.h"

void	update_tok(char *line, t_token **tok, int *q_flag, t_token *lst_tok)
{
	if (*line == '"' && *q_flag == 0)
		*q_flag = 2;
	else if (*line == '\'' && *q_flag == 0)
		*q_flag = 1;
	else if ((*line == '"' && *q_flag == 2)
		|| (*line == '\'' && *q_flag == 1))
		*q_flag = 0;
	else if (*line != ' ' && !is_op(line) && !is_fb(line))
		(*tok)->content = ft_addchr((*tok)->content, *line, lst_tok);
}

void	add_rest(t_token **lst_tok, t_token *tok, int q_flag)
{
	if (q_flag == 0 && tok != NULL && tok->content[0] != '\0')
		add_back_tok(lst_tok, tok);
	else if (q_flag == 1 || q_flag == 2)
	{
		free(tok);
		failure_parse("Error: unclosed quote", *lst_tok);
	}
}

void	meta_tok(char *line, int *i, t_token **lst_tok)
{
	char	*tok;

	tok = malloc(sizeof(char));
	if (!tok)
		failure_parse("Error: malloc", *lst_tok);
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
		tok = ft_addchr(tok, line[*i], *lst_tok);
		add_back_tok(lst_tok, new_token(tok, get_type(tok)));
		*i += 1;
	}
	else
		free(tok);
}

void	is_true_op(char *line, int *i, t_token **tok, t_token **lst_tok)
{
	if (is_op(line + *i) || is_fb(line + *i))
	{
		if (is_op(line + *i) || is_fb(line + *i) == DREDIR2
			|| is_fb(line + *i) == DREDIR2)
		{
			(*tok)->content = ft_addchr((*tok)->content, line[(*i)++],
					*lst_tok);
			(*tok)->content = ft_addchr((*tok)->content, line[(*i)++],
					*lst_tok);
		}
		else
			(*tok)->content = ft_addchr((*tok)->content, line[(*i)++],
					*lst_tok);
		(*tok)->type = 0;
	}
	else
		(*tok)->content = ft_addchr((*tok)->content, line[*i], *lst_tok);
}

t_token	*tokenize(char *line, t_env *lst_env)
{
	t_token	*lst_tok;
	t_token	*tok;
	int		q_flag;
	int		i;

	tok = init_param(&lst_tok, &q_flag, &i);
	while ((size_t)i < ft_strlen(line))
	{
		if (lst_tok && last_elem(lst_tok)->type == DREDIR2)
			quoted(line + i, &lst_tok);
		if (*(line + i) == '$' && q_flag != 1
			&& !is_heredoc(lst_tok))
		{
			line = expansion(line, &i, lst_env);
			if (!line)
				return (NULL);
		}	
		else if (is_op(line + i) || is_fb(line + i) || line[i] == ' ')
		{
			if (q_flag == 0 && tok != NULL && tok->content[0] != '\0')
				doo(&lst_tok, &tok);
			else if (q_flag == 1 || q_flag == 2)
				is_true_op(line, &i, &tok, &lst_tok);
			meta_tok(line, &i, &lst_tok);
		}
		update_tok(line + i, &tok, &q_flag, lst_tok);
		if (line[i] != '\0' && !is_op(line + i) && !is_fb(line + i))
			i++;
	}
	return (add_rest(&lst_tok, tok, q_flag), check_tok(lst_tok), lst_tok);
}

void	merge(t_token **res, t_token *cmd, t_token *redirs)
{
	t_token	*tmp;

	tmp = NULL;
	if (!*res)
	{
		*res = cmd;
		tmp = last_elem(*res);
		if (!tmp)
			*res = redirs;
		else
			tmp->next = redirs;
		return ;
	}
	tmp = last_elem(*res);
	tmp->next = cmd;
	tmp = last_elem(*res);
	tmp->next = redirs;
}

t_token	*tokenize_bise(t_token *tok)
{
	t_token *cmds;
	t_token	*redirs;
	t_token	*res;
	t_token	*tmp;

	tmp = tok;
	res = NULL;
	cmds = NULL;
	redirs = NULL;
	if (!tok)
		return (NULL);
	while (tmp)
	{
		if (tmp->type == CMD)
			add_back_tok(&cmds, new_token(ft_strdup(tmp->content), tmp->type));
		else if (tmp->type >= REDIR && tmp->type <= DREDIR2_E && tmp->next)
		{
			add_back_tok(&redirs, new_token(ft_strdup(tmp->next->content),
					tmp->type));
			tmp = tmp->next;
		}
		else
		{
			merge(&res, cmds, redirs);
			cmds = NULL;
			redirs = NULL;
			add_back_tok(&res, new_token(ft_strdup(tmp->content), tmp->type));
		}
		tmp = tmp->next;
	}
	merge(&res, cmds, redirs);
	cmds = NULL;
	redirs = NULL;
	if (tok)
		free_lst_tok(&tok);
	return (res);
}

t_token	*tokenize_crise(t_token *tok)
{
	t_token	*tmp;
	char	*pwd;
	t_token	*res;
	int		flag;

	pwd = getcwd(NULL, 0);
	tmp = tok;
	flag = 0;
	res = NULL;
	while (tmp)
	{
		if (tmp->type == CMD && tmp->next)
		{
			add_back_tok(&res, new_token(ft_strdup(tmp->content), tmp->type));
			tmp = tmp->next;
			while (tmp && tmp->type == CMD)
			{
				process_wild(tmp->content, pwd, &res, &flag);
				if (flag == 0)
					add_back_tok(&res, new_token(ft_strdup(tmp->content), tmp->type));
				else if (flag == 1)
					flag = 0;
				tmp = tmp->next;
			}
		}
		else
		{
			add_back_tok(&res, new_token(ft_strdup(tmp->content), tmp->type));
			tmp = tmp->next;
		}
	}
	if (tok)
		free_lst_tok(&tok);
	return (res);
}

/*------------------------------------FUNCTIONS TO DELETE-----------------------------------------------------*/

void print_list_tok(t_token *lst_tok)
{
	t_token *temp;
	int		size = 0;

	printf("-------------------------------\n");
	temp = lst_tok;
	while (temp)
	{
		printf("[%s, %d]->", temp->content, temp->type);
		//printf("[%s]->", temp->content);
		temp = temp->next;
		size++;
		//printf("\n");
	}
	printf("\n-----size of lst_tok : %d------\n", size);
}

// int	main(int ac, char **av, char **env)
// {
// 	t_token	*lst_tok;
// 	t_token *lst_bise;
// 	t_env	*lst_env;

// 	(void)av;
// 	(void)ac;
// 	// lst_tok = tokenize("ech\'o\' hello&&(echo \"world\" || echo \"hello world\")");
// 	//lst_tok = tokenize("ech\'o\' hello&&(echo \"world\" || echo \"hello world\")");
// 	//lst_tok = tokenize("(((echo '$bye')) && cd dossier||echo $user ) | cat -e");	
// 	// ( smpl_cmd && smpl_cmd || smpl_cmd ) | smpl_cmd
// 	//lst_tok = tokenize("(echo '$bye' && echo $nym || echo $user ) | cat -e");
// 	lst_env = spy_env(env);
// 	//char *str = ft_strdup("echo \"'$user'$USER$USER'$USER'\"$user\"\" && echo $USER | cat -e || (export vat=42 && echo lol)");
// 	//char *str = ft_strdup("cat << $USER << $US'ER' << \"$USER\" > f << \"EOF E\"OF | cat << EOF > g");
// 	//char *str = ft_strdup("(ls && cat) << EOF");
// 	//char *str = ft_strdup(" ");
// 	//char *str = ft_strdup("ls > f -la");
// 	//char *str = ft_strdup("((ls > f -la) && echo a >> f) << EOF | cat");
// 	lst_tok = tokenize(str, lst_env);
// 	print_list_tok(lst_tok);
// 	lst_bise = tokenize_bise(lst_tok);
// 	print_list_tok(lst_bise);
// 	// lst_tok = tokenize("echo hello is $USER");
// 	//printf("Testing 1 2 1 2, the mic is on\n");
// 	//print_list_tok(lst_tok);
// 	return (0);
// }
/*------------------------------------FUNCTIONS TO DELETE-----------------------------------------------------*/