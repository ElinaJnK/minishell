#include "minishell.h"

int	fill_cmd(t_cmd *cmd, t_token **t)
{
	t_token	*tmp;
	int		i;

	tmp = (*t)->next;
	i = 0;
	if (init_op(cmd, *t) == EXIT_FAILURE)
		return (EXIT_FAILURE);
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

int	open_files(t_token **t, t_cmd *cmd, int *pipe_fds, t_env *env)
{
	if (access((*t)->content, R_OK) != 0)
	{
		if (errno == ENOENT)
		{
			ft_putstr_fd("bash: ", cmd->output);
			ft_putstr_fd((*t)->content, cmd->output);
			ft_putstr_fd(": No such file or directory\n", cmd->output);
		}
		else
		{
			ft_putstr_fd("bash: ", cmd->output);
			ft_putstr_fd((*t)->content, cmd->output);
			ft_putstr_fd(": Permission denied\n", cmd->output);
		}
	}
	if ((*t)->type == REDIR)
		cmd->output = open((*t)->content, O_WRONLY | O_CREAT | O_TRUNC,
				0644);
	else if ((*t)->type == REDIR2)
		cmd->input = open((*t)->content, O_RDONLY, 0644);
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
	return (EXIT_SUCCESS);
}

int	fill_redir(t_cmd *cmd, t_token **t, t_env *env)
{
	int		pipe_fds[2];
	t_token	*prev;

	if (init_op(cmd, NULL) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	prev = *t;
	while (*t && !((*t)->type >= AND && (*t)->type <= PIPE)
		&& !((*t)->type >= OPEN_PAR && (*t)->type <= CLOSE_PAR))
	{
		if (open_files(t, cmd, pipe_fds, env) == EXIT_FAILURE)
			return (EXIT_FAILURE);
		if (cmd->output < 0 || cmd->input < 0)
			return (EXIT_FAILURE);
		cmd->type = (*t)->type;
		prev = *t;
		*t = (*t)->next;
	}
	cmd->content = ft_strdup(prev->content);
	return (EXIT_SUCCESS);
}

int	define_types(t_token **t, t_cmd *cmd, t_env *env)
{
	if ((*t)->type == CMD)
	{
		if (fill_cmd(cmd, t) == EXIT_FAILURE)
			return (EXIT_FAILURE);
	}
	else if ((*t)->type >= REDIR && (*t)->type <= DREDIR2_E)
	{
		if (fill_redir(cmd, t, env) == EXIT_FAILURE)
			return (EXIT_FAILURE);
	}
	else if ((*t)->type != CMD)
	{
		if (init_op(cmd, *t) == EXIT_FAILURE)
			return (EXIT_FAILURE);
		*t = (*t)->next;
	}
	return (EXIT_SUCCESS);
}

t_cmd	*transform_into_tab(t_token *t, int *count, t_env *env)
{
	t_cmd	*cmd;
	t_token	*tmp;
	int		i;

	if (!t)
		return (NULL);
	cmd = malloc(sizeof(t_cmd) * (lst_size_tok(t) + 1));
	if (!cmd)
		return (failure_parse("Malloc allocation failure", t, NULL), NULL);
	i = 0;
	tmp = t;
	while (t)
	{
		if (define_types(&t, &cmd[i], env) == EXIT_FAILURE)
			return (free_cmds(cmd, *count), free_lst_tok(&tmp), NULL);
		i++;
	}
	
	if (tmp)
		free_lst_tok(&tmp);
	*count = i;
	cmd[i].content = NULL;
	cmd[i].type = -1;
	return (cmd);
}
