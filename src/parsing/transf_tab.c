/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   transf_tab.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ejankovs <ejankovs@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/20 04:18:51 by ksadykov          #+#    #+#             */
/*   Updated: 2023/08/20 05:50:311 by ejankovs         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

int	open_files(t_token **t, t_cmd *cmd, t_all *all, t_token **lst_err,
	t_token *tmp)
{
	int	fd;

	if ((*t)->type == DREDIR2 || (*t)->type == DREDIR2_E)
	{
		if (cmd->input != STDIN_FILENO)
			close(cmd->input);
		fd = open("/tmp/minishell", O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (fd < 0)
			return (EXIT_FAILURE);
		if (open_here_doc(fd, *t, all, tmp)
			== EXIT_FAILURE || *exit_status() == 130)
			return (close(fd), EXIT_FAILURE);
		close(fd);
		fd = open("/tmp/minishell", O_RDONLY, 0644);
		cmd->input = fd;
	}
	else
	{
		if ((*t)->type == REDIR)
		{
			if (cmd->output != STDOUT_FILENO)
				close(cmd->output);
			cmd->output = open((*t)->content, O_WRONLY | O_CREAT | O_TRUNC,
					0644);
		}
		else if ((*t)->type == REDIR2)
		{
			if (cmd->input != STDIN_FILENO)
				close(cmd->input);
			cmd->input = open((*t)->content, O_RDONLY, 0644);
		}
		else if ((*t)->type == DREDIR)
		{
			if (cmd->output != STDOUT_FILENO)
				close(cmd->output);
			cmd->output = open((*t)->content, O_WRONLY | O_CREAT | O_APPEND,
					0644);
		}
		add_error((*t)->content, cmd->output, lst_err);
	}
	return (EXIT_SUCCESS);
}

int	fill_redir(t_cmd *cmd, t_token **t, t_all *all, t_token *tmp)
{
	t_token	*prev;
	t_token	*lst_err;

	lst_err = NULL;
	if (init_op(cmd, NULL) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	prev = *t;
	while (*t && !((*t)->type >= AND && (*t)->type <= PIPE)
		&& !((*t)->type >= OPEN_PAR && (*t)->type <= CLOSE_PAR))
	{
		if (open_files(t, cmd, all, &lst_err, tmp) == EXIT_FAILURE)
			return (EXIT_FAILURE);
		cmd->lst_err = lst_err;
		cmd->type = (*t)->type;
		prev = *t;
		*t = (*t)->next;
	}
	cmd->content = ft_strdup(prev->content);
	return (EXIT_SUCCESS);
}

int	define_types(t_token **t, t_cmd *cmd, t_all *all, t_token *tmp)
{
	if ((*t)->type == CMD)
	{
		if (fill_cmd(cmd, t) == EXIT_FAILURE)
			return (EXIT_FAILURE);
	}
	else if ((*t)->type >= REDIR && (*t)->type <= DREDIR2_E)
	{
		if (fill_redir(cmd, t, all, tmp) == EXIT_FAILURE)
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

t_cmd	*transform_into_tab(t_token *t, int *count, t_all **all)
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
	(*all)->cmd = cmd;
	while (t)
	{
		if (define_types(&t, &cmd[i], *all, tmp) == EXIT_FAILURE)
		{
			if (tmp)
				free_lst_tok(&tmp);
			return (free_cmds(cmd, *count), NULL);
		}
		i++;
	}
	if (tmp)
	{
		free_lst_tok(&tmp);
		tmp = NULL;
	}
	*count = i;
	cmd[i].content = NULL;
	cmd[i].type = -1;
	return (cmd);
}
