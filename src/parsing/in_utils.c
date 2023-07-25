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
	cmd->content = (*t)->content;
	cmd->type = (*t)->type;
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
		cmd->args[i] = (*t)->content;
		*t = (*t)->next;
		i++;
	}
	cmd->args[i] = NULL;
}

void	init_op(t_cmd *cmd, t_token *t)
{
	cmd->content = t->content;
	cmd->type = t->type;
	cmd->args = NULL;
	cmd->nb_args = 0;
}

void	fill_redir(t_cmd *cmd, t_token **t)
{
	int		pipe_fds[2];

	while (*t && !((*t)->type >= AND && (*t)->type <= PIPE)
		&& !((*t)->type >= OPEN_PAR && (*t)->type <= CLOSE_PAR))
	{
		if ((*t)->type == REDIR || (*t)->type == REDIR2)
		{
			cmd->content = (*t)->content;
			cmd->type = (*t)->type;
		}
		else if ((*t)->type == DREDIR)
		{
			//append
		}
		else if ((*t)->type == DREDIR2 || (*t)->type == DREDIR2_E)
		{
			open_here_doc(pipe_fds, (*t)->content);
			cmd->type = (*t)->type;
		}
		*t = (*t)->next;
	}
	cmd->input = get_content(pipe_fds[0]);
	cmd->output = get_content(pipe_fds[1]);
}


t_cmd	*transform_into_tab(t_token *t, int *count)
{
	t_cmd	*cmd;
	t_token	*tmp;
	int		i;

	(void)tmp;
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
			fill_redir(&cmd[i], &t);
		else if (t->type != CMD)
		{
			init_op(&cmd[i], t);
			t = t->next;
		}
		i++;
	}
	//free_lst_tok(&tmp);
	*count = i;
	cmd[i].content = NULL;
	cmd[i].type = -1;
	return (cmd);
}
