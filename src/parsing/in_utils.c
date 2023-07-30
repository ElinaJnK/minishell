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

int	fill_cmd(t_cmd *cmd, t_token **t)
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
		return (EXIT_FAILURE);
	while (*t && (*t)->type == CMD && i < cmd->nb_args + 1)
	{
		cmd->args[i] = ft_strdup((*t)->content);
		if (!cmd->args[i])
			return (EXIT_FAILURE);
		*t = (*t)->next;
		i++;
	}
	cmd->args[i] = NULL;
	return (EXIT_SUCCESS);
}

int	init_op(t_cmd *cmd, t_token *t)
{
	cmd->content = ft_strdup(t->content);
	if (!cmd->content)
		return (EXIT_FAILURE);
	cmd->type = t->type;
	cmd->output = STDOUT_FILENO;
	cmd->input = STDIN_FILENO;
	cmd->args = NULL;
	cmd->nb_args = 0;
	return (EXIT_SUCCESS);
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
				return (EXIT_FAILURE);
			cmd->input = pipe_fds[0];
		}
		if (cmd->output < 0 || cmd->input < 0)
			return (EXIT_FAILURE);
		cmd->type = (*t)->type;
		cmd->content = ft_strdup((*t)->content);
		*t = (*t)->next;
	}
	return (EXIT_SUCCESS);
}

t_cmd	*transform_into_tab(t_token *t, int *count, t_env *env)
{
	t_cmd	*cmd;
	t_token	*tmp;
	int		i;

	cmd = malloc(sizeof(t_cmd) * (lst_size_tok(t) + 1));
	if (!cmd)
		return (failure_parse("Malloc allocation failure", t), NULL);
	i = 0;
	tmp = t;
	while (t)
	{
		if (t->type == CMD)
		{
			if (fill_cmd(&cmd[i], &t) == EXIT_FAILURE)
				return (free_cmds(cmd, *count), free_lst_tok(&tmp), NULL);
		}
		else if (t->type >= REDIR && t->type <= DREDIR2_E)
		{
			if (fill_redir(&cmd[i], &t, env) == EXIT_FAILURE)
				return (free_cmds(cmd, *count), free_lst_tok(&tmp), NULL);
		}
		else if (t->type != CMD)
		{
			if (init_op(&cmd[i], t) == EXIT_FAILURE)
				return (free_cmds(cmd, *count), free_lst_tok(&tmp), NULL);
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
