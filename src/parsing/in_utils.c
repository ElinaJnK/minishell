#include "minishell.h"

int	lst_size_tok(t_token *lst)
{
	int		i;
	t_token	*tmp;

	i = 0;
	tmp = lst;
	while (tmp)
	{
		i++;
		tmp = tmp->next;
	}
	return (i);
}

void	fill_cmd(t_cmd *cmd, t_token **t)
{
	t_token	*tmp;
	int		i;

	tmp = (*t)->next;
	i = 0;
	cmd->content = ft_strdup((*t)->content);
	cmd->type = (*t)->type;
	cmd->output = 1;
	cmd->input = 0;
	cmd->nb_args = 0;
	while (tmp && tmp->type == CMD)
	{
		(cmd->nb_args)++;
		tmp = tmp->next;
	}
	cmd->args = (char **)malloc(sizeof(char *) * (cmd->nb_args + 2));
	if (!cmd->args)
		return (failure_parse("malloc in abr creation", *t));
	while (*t && (*t)->type == CMD && i < cmd->nb_args + 1)
	{
		cmd->args[i] = ft_strdup((*t)->content);
		*t = (*t)->next;
		i++;
	}
	cmd->args[i] = NULL;
}

void	init_op(t_cmd *cmd, t_token *t)
{
	cmd->content = ft_strdup(t->content);
	cmd->type = t->type;
	cmd->output = 1;
	cmd->input = 0;
	cmd->args = NULL;
	cmd->nb_args = 0;
}

int	fill_redir(t_cmd *cmd, t_token **t, t_env *env)
{
	int	pipe_fds[2];

	cmd->content = NULL;
	cmd->output = 1;
	cmd->input = 0;
	cmd->args = NULL;
	cmd->nb_args = 0;
	while (*t && !((*t)->type >= AND && (*t)->type <= PIPE)
		&& !((*t)->type >= OPEN_PAR && (*t)->type <= CLOSE_PAR))
	{
		if ((*t)->type == REDIR)
			cmd->output = open((*t)->content, O_WRONLY | O_CREAT | O_TRUNC,
					0644);
		else if ((*t)->type == REDIR2)
			cmd->input = open((*t)->content, O_RDONLY | O_CREAT,
					0644);
		else if ((*t)->type == DREDIR)
			cmd->output = open((*t)->content, O_WRONLY | O_CREAT | O_APPEND,
					0644);
		else if ((*t)->type == DREDIR2 || (*t)->type == DREDIR2_E)
		{
			if (open_here_doc(pipe_fds, (*t)->content, (*t)->type, env)
				== EXIT_FAILURE)
				return (failure_parse("open_here_doc failed"), EXIT_FAILURE); 
			cmd->input = pipe_fds[0];
		}
		if (cmd->output < 0 || cmd->input < 0)
			return (failure_parse("open failed"), EXIT_FAILURE);
		cmd->type = (*t)->type;
		cmd->content = ft_strdup((*t)->content);
		*t = (*t)->next;
	}
	return (EXIT_SUCCESS);
}

// int	in_lst(t_token *seen, int type)
// {
// 	t_token	*tmp;

// 	tmp = seen;
// 	while (tmp)
// 	{
// 		if (tmp->type == type)
// 			return (1);
// 	}
// 	return (0);
// }

// int	get_len_lst(t_token *t)
// {
// 	t_token	*tmp;
// 	t_token	*seen;
// 	int		size;

// 	tmp = t;
// 	size = 0;
// 	seen = NULL;
// 	while (tmp)
// 	{
// 		// count the redirs
// 		while (tmp && !(tmp->type >= AND && t->type <= PIPE)
// 			&& !(t->type >= OPEN_PAR && t->type <= CLOSE_PAR))
// 		{
// 			if (t->type == REDIR && in_lst(seen, t->type) == 0)
// 			{
// 				size++;
// 				add_back_tok(&seen, new_token(ft_strdup(t->content), t->type));
// 			}
// 			else if (t->type == REDIR2 && in_lst(seen, t->type) == 0)
// 			{
// 				size++;
// 				add_back_tok(&seen, new_token(ft_strdup(t->content), t->type));
// 			}
// 			else if (t->type == DREDIR && in_lst(seen, t->type) == 0)
// 			{
// 				size++;
// 				add_back_tok(&seen, new_token(ft_strdup(t->content), t->type));
// 			}
// 			else if ((t->type == DREDIR2 || t->type == DREDIR2_E) && in_lst(seen, t->type) == 0)
// 			{
// 				size++;
// 				add_back_tok(&seen, new_token(ft_strdup(t->content), t->type));
// 			}
// 			else
// 				size++;
// 			tmp->next;
// 		}
// 		size++;
// 		if (seen)
// 			free_lst_tok(&seen);
// 		tmp = tmp->next;
// 	}
// 	if (seen)
// 		free_lst_tok(&seen);
// 	return (size);
// }

t_cmd	*transform_into_tab(t_token *t, int *count, t_env *env)
{
	t_cmd	*cmd;
	t_token	*tmp;
	int		i;

	//printf("Does it work ?? : %d\n", get_len_lst(t));
	cmd = malloc(sizeof(t_cmd) * (lst_size_tok(t) + 1));
	if (!cmd)
		return (failure_parse("cmd is NULL in-utils", t), NULL);
	i = 0;
	tmp = t;
	while (t)
	{
		if (t->type == CMD)
			fill_cmd(&cmd[i], &t);
		else if (t->type >= REDIR && t->type <= DREDIR2_E)
			fill_redir(&cmd[i], &t, env);
		else if (t->type != CMD)
		{
			init_op(&cmd[i], t);
			t = t->next;
		}
		i++;
	}
	if (tmp)
		free_lst_tok(&tmp);
	*count = i;
	cmd[i].content = NULL;
	cmd[i].type = -1;
	return (cmd);
}
