#include "minishell.h"

int	lst_size(t_token *lst)
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
	if (!cmd->nb_args)
		return ;
	cmd->args = (char **)malloc(sizeof(char *) * cmd->nb_args);
	if (!cmd->args)
		return (failure_parse("malloc in abr creation", *t));
	while (t && (*t)->type == CMD && i < cmd->nb_args)
	{
		*t = (*t)->next;
		cmd->args[i] = (*t)->content;
		i++;
	}
}

void	init_op(t_cmd *cmd, t_token *t)
{
	cmd->content = t->content;
	cmd->type = t->type;
	cmd->args = NULL;
	cmd->nb_args = 0;
}

t_cmd	*transform_into_tab(t_token *t, int *count)
{
	t_cmd	*cmd;
	t_token	*tmp;
	int		i;

	cmd = malloc(sizeof(t_cmd) * (lst_size(t) + 1));
	if (!cmd)
		return (failure_parse("cmd is NULL in-utils", t), NULL);
	i = 0;
	while (t)
	{
		if (t->type == CMD)
			fill_cmd(&cmd[i], &t);
		else
			init_op(&cmd[i], t);
		tmp = t;
		t = t->next;
		free(tmp);
		i++;
	}
	*count = i;
	cmd[i].content = NULL;
	cmd[i].type = -1;
	i = 0;
	return (cmd);
}
