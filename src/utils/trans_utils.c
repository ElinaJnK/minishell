/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   trans_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ejankovs <ejankovs@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/20 20:05:42 by ksadykov          #+#    #+#             */
/*   Updated: 2023/08/20 20:05:42 by ejankovs         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	finish_trans(t_cmd	*cmd, t_token *tmp, int *count, int i)
{
	if (tmp)
	{
		free_lst_tok(&tmp);
		tmp = NULL;
	}
	*count = i;
	cmd[i].content = NULL;
	cmd[i].type = -1;
}

void	redirs(t_token **t, t_cmd *cmd)
{
	char	*ct;

	ct = (*t)->content;
	if ((*t)->type == REDIR)
	{
		if (cmd->output != STDOUT_FILENO && cmd->output > 0)
			close(cmd->output);
		cmd->output = open(ct, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	}
	else if ((*t)->type == REDIR2)
	{
		if (cmd->input != STDIN_FILENO && cmd->input > 0)
			close(cmd->input);
		cmd->input = open(ct, O_RDONLY, 0644);
	}
	else if ((*t)->type == DREDIR)
	{
		if (cmd->output != STDOUT_FILENO && cmd->output > 0)
			close(cmd->output);
		cmd->output = open(ct, O_WRONLY | O_CREAT | O_APPEND, 0644);
	}
	add_error(ct, cmd->output, &(cmd->lst_err));
}
